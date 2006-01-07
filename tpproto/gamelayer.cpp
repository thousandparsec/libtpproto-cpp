/*  GameLayer class
 *
 *  Copyright (C) 2005-2006  Lee Begg and the Thousand Parsec Project
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <iostream>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef VERSION
#define VERSION "0.0.0"
#endif

#include "tpsocket.h"
#include "framefactory.h"
#include "protocollayer.h"
#include "logger.h"
#include "gamestatuslistener.h"
#include "asyncframelistener.h"
#include "silentlogger.h"
#include "framecodec.h"
#include "framebuilder.h"
#include "tcpsocket.h"
#ifdef HAVE_LIBGNUTLS
#include "tpssocket.h"
#include "httpssocket.h"
#endif

// caches
#include "cachemethod.h"
#include "objectcache.h"
#include "playercache.h"
#include "boardcache.h"
#include "resourcecache.h"
#include "categorycache.h"
#include "designcache.h"
#include "componentcache.h"
#include "propertycache.h"

// Frame Types

#include "okframe.h"
#include "failframe.h"
#include "sequence.h"
#include "connect.h"
#include "login.h"
#include "object.h"
#include "board.h"
#include "getmessage.h"
#include "message.h"
#include "removemessage.h"
#include "gettime.h"
#include "timeremaining.h"
#include "order.h"
#include "getorder.h"
#include "removeorder.h"
#include "orderdesc.h"
#include "getorderdesc.h"
#include "featuresframe.h"
#include "redirect.h"
#include "resourcedesc.h"
#include "player.h"
#include "category.h"
#include "design.h"
#include "component.h"
#include "property.h"
#include "probeorder.h"
#include "adddesign.h"
#include "modifydesign.h"
#include "removedesign.h"

#include "gamelayer.h"

namespace TPProto {

    /*! \brief A AsyncFrameListener for GameLayer.
    Internally used by GameLayer to get async frames.
    */
    class GameLayerAsyncFrameListener : public AsyncFrameListener{
    public:
        
        virtual ~GameLayerAsyncFrameListener(){}
        
        /*! \brief Set the GameLayer
        \param gl The GameLayer to use.
        */
        void setGameLayer(GameLayer* gl){
            layer = gl;
        }
        
        void recvTimeRemaining(TimeRemaining* trf){
            if(trf->getTimeRemaining() == 0){
                layer->status = gsEOTInProgress;
                if(layer->statuslistener != NULL){
                    layer->statuslistener->eotStarted();
                    layer->statuslistener->timeToEot(trf->getTimeRemaining());
                }
            }else{
                if(layer->status == gsEOTInProgress){
                    layer->status = gsLoggedIn;
                    if(layer->statuslistener != NULL){
                        layer->statuslistener->eotEnded();
                        layer->updateCaches();
                    }
                }
                if(layer->statuslistener != NULL){
                    layer->statuslistener->timeToEot(trf->getTimeRemaining());
                }
            }
        }
        
    private:
        GameLayer* layer;
    };

    /*! \brief Constructs object and sets up defaults.

    Defaults are
        - The default ProtocolLayer
        - Disconnected state.
        - "Unknown client" for the client string
    */
    GameLayer::GameLayer() : protocol(NULL), logger(NULL), statuslistener(NULL), status(gsDisconnected),
            clientid("Unknown client"), serverfeatures(NULL), asyncframes(new GameLayerAsyncFrameListener()),
            objectcache(new ObjectCache()), playercache(new PlayerCache()), boardcache(new BoardCache()),
            resourcecache(new ResourceCache()), categorycache(new CategoryCache()),
            designcache(new DesignCache()), componentcache(new ComponentCache()),
            propertycache(new PropertyCache()){
        protocol = new ProtocolLayer();
        logger = new SilentLogger();
        sock = NULL;
        asyncframes->setGameLayer(this);
        protocol->getFrameCodec()->setAsyncFrameListener(asyncframes);
        objectcache->setProtocolLayer(protocol);
        playercache->setProtocolLayer(protocol);
        boardcache->setProtocolLayer(protocol);
        resourcecache->setProtocolLayer(protocol);
        categorycache->setProtocolLayer(protocol);
        designcache->setProtocolLayer(protocol);
        componentcache->setProtocolLayer(protocol);
        propertycache->setProtocolLayer(protocol);
    }

    /*! \brief Destructor.
    */
    GameLayer::~GameLayer(){
        if(status != gsDisconnected){
            sock->disconnect();
        }
        delete protocol;

        if(serverfeatures != NULL){
            delete serverfeatures;
        }
        delete asyncframes;
        delete objectcache;
        delete playercache;
        delete boardcache;
        delete resourcecache;
        delete categorycache;
        delete designcache;
        delete componentcache;
        delete propertycache;
    }

    /*! \brief Sets the client string.

    The client string can be set to anything.  The perferred format is
    "name/version".  The library name and version is added the the client
    string later.
    \param name The client name string.
    */
    void GameLayer::setClientString(const std::string & name){
        clientid = name;
    }

    /*! \brief Sets the Logger to use
    \param nlog The new Logger to use.
    */
    void GameLayer::setLogger(Logger* nlog){
        logger = nlog;
        protocol->getFrameCodec()->setLogger(nlog);
    }

    /*! \brief Sets the GameStatusListener to use.
    \param gsl The new GameStatusListener to use.
    */
    void GameLayer::setGameStatusListener(GameStatusListener* gsl){
        statuslistener = gsl;
    }

    /*! \brief Gets the state of the game.
    \return The GameStatus enum value for the current state.
    */
    GameStatus GameLayer::getStatus(){
        if(sock == NULL || !sock->isConnected()){
            status = gsDisconnected;
            if(statuslistener != NULL)
                statuslistener->disconnected();
        }
        return status;
    }

    /*! \brief Sets the CacheMethod for the caches to use.
    Changes the default CacheMethod used, defaults to CacheNoneMethod.
    \param prototype A CacheMethod that will be cloned for each Cache to use.
    */
    void GameLayer::setCacheMethod(CacheMethod* prototype){
        objectcache->setCacheMethod(prototype->clone());
        boardcache->setCacheMethod(prototype->clone());
        playercache->setCacheMethod(prototype->clone());
        resourcecache->setCacheMethod(prototype->clone());
        categorycache->setCacheMethod(prototype->clone());
        designcache->setCacheMethod(prototype->clone());
        componentcache->setCacheMethod(prototype->clone());
        propertycache->setCacheMethod(prototype->clone());
    }

    /*! \brief Connects to the given address url
    This method connects to the server given as the address. The types of url
    supported are tp, tps, https and http. Tps and https depend on TLS being enabled.
    \param address The URL to connect to.
    \return True if connected, false otherwise.
    */
    bool GameLayer::connect(const std::string& address){
        if(status != gsDisconnected){
            logger->warning("Already connected, ignoring connection attempt");
            return false;
        }
        std::string type, host, port;
        //parse address to type, host, and port
        size_t tpos = address.find("://");
        if(tpos != address.npos){
            type = address.substr(0, tpos);
            tpos += 3;
        }else{
            tpos = 0;
        }
        size_t ppos = address.rfind(':');
        size_t bpos = address.rfind(']');
        if(ppos != std::string::npos && ppos > tpos){
            if(bpos == std::string::npos || ppos > bpos){
                port = address.substr(ppos + 1);
            }else{
                ppos = std::string::npos;
            }
        }else{
            ppos = std::string::npos;
        }
        host = address.substr(tpos, ppos - tpos);

        TPSocket *sock = NULL;
        if(type.empty() || type == "tp"){
            if(port.empty()){
                port = "6923";
            }
            sock = new TcpSocket();
            static_cast<TcpSocket*>(sock)->setServerAddr(host.c_str(), port.c_str());
        }else if(type == "tps"){
            if(port.empty()){
                port = "6924";
            }
#ifdef HAVE_LIBGNUTLS
            sock = new TpsSocket();
            static_cast<TpsSocket*>(sock)->setServerAddr(host.c_str(), port.c_str());
#endif
        }else if(type == "http"){
            if(port.empty()){
                port = "80";
            }
            //TODO
        }else if(type == "https"){
            if(port.empty()){
                port = "443";
            }
#ifdef HAVE_LIBGNUTLS
            sock = new HttpsSocket();
            static_cast<HttpsSocket*>(sock)->setServerAddr(host.c_str(), port.c_str());
            //need to have set proxy address already (is static)
#endif
        }else{
            logger->error("Type of connection to create was not known");
            return false;
        }
        if(sock != NULL){
            return connect(sock);
        }else{
            return false;
        }
    }

    /*! \brief Connects using a given TPSocket.
    Connects to a server using a given TPSocket.
    \param nsock The TPSocket to connect using.
    \return True if connected, false otherwise.
    */
    bool GameLayer::connect(TPSocket* nsock){
        if(status != gsDisconnected){
            logger->warning("Already connected, ignoring connection attempt");
            return false;
        }
        sock = nsock;
        protocol->getFrameCodec()->setSocket(sock);
        if(sock->connect()){
            logger->debug("Connection opened");
            status = gsConnecting;
            Connect * cf = protocol->getFrameFactory()->createConnect();
            cf->setClientString(std::string("libtpproto-cpp/") + VERSION + " " + clientid);
            uint32_t seqnum = protocol->getFrameCodec()->sendFrame(cf);
            delete cf;
            
            std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
            Frame * reply = NULL;
            if(replies.size() >= 1){
                reply = replies.front();
            }
            
            if(reply != NULL && reply->getType() == ft02_OK){
                // expect OK back
                //  or maybe error
                status = gsConnected;
                if(statuslistener != NULL)
                    statuslistener->connected();
                logger->info("Connected");
                delete reply;
                updateCaches();
                return true;
            }else if(reply != NULL && reply->getType() == ft03_Redirect){
                //signal we are redirecting 
                if(statuslistener != NULL)
                    statuslistener->redirected(static_cast<Redirect*>(reply)->getUrl());
                bool rtv = connect(static_cast<Redirect*>(reply)->getUrl());
                delete reply;
                return rtv;
            }else{
                status = gsDisconnected;
                logger->error("Could not connect");
                sock->disconnect();
                if(reply != NULL)
                    delete reply;
            }
        }else{
            logger->error("Could not open socket to server");
        }
        return false;
    }

    /*! \brief Logs in to the server.
    
    Sends a Login Frame to the server and waits for a reply.
    \param username The username to connect as.
    \param password The password of the account of the username.
    \return True if successful, false otherwise.
    */
    bool GameLayer::login(const std::string &username, const std::string &password){
        if(status == gsConnected && sock->isConnected()){
            Login * login = protocol->getFrameFactory()->createLogin();
            login->setUser(username);
            login->setPass(password);
            uint32_t seqnum = protocol->getFrameCodec()->sendFrame(login);
            delete login;
            
            std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
            Frame * reply = NULL;
            if(replies.size() >= 1){
                reply = replies.front();
            }
            
            if(reply != NULL && reply->getType() == ft02_OK){
                // expect OK back
                //  or maybe error
                status = gsLoggedIn;
                if(statuslistener != NULL)
                    statuslistener->loggedIn();
                logger->info("Logged in");
                delete reply;
                return true;
            }else{
                logger->warning("Did not log in");
                if(reply != NULL)
                    delete reply;
            }
            
        }
        if(!sock->isConnected()){
            status = gsDisconnected;
            if(statuslistener != NULL)
                statuslistener->disconnected();
        }
        return false;
    }

    /*! \brief Disconnects from server.
    
    Closes the underlying TPSocket.
    */
    void GameLayer::disconnect(){
        if(status != gsDisconnected && sock != NULL){
            sock->disconnect();
            logger->info("Disconnected");
            if(statuslistener != NULL)
                statuslistener->disconnected();
        }
        status = gsDisconnected;
    }

    /*! \brief Tells all the caches to update.
    Called automatically after logged in, and after EOT has finished.
    Call if you want the caches to be updated.
    */
    void GameLayer::updateCaches(){
        objectcache->update();
        boardcache->update();
        playercache->update();
        resourcecache->update();
        categorycache->update();
        designcache->update();
        componentcache->update();
        propertycache->update();
    }


    /*! \brief Gets objectids from the server.
    
    Gets the complete list of Object ids.
    \return The set of object id.
    */
    std::set<uint32_t> GameLayer::getObjectIds(){
        return objectcache->getObjectIds();
    }

    /*! \brief Gets an object from the server.
    
    Gets an object from the server and returns the Object.
    \param obid The Object id of the object to get.
    \return The Object.
    */
    Object* GameLayer::getObject(uint32_t obid){
        return objectcache->getObject(obid);
    }

    /*! \brief Gets the Universe Object.

    A handy method to get the Universe Object.
    \return The Object of the Universe.
    */
    Object* GameLayer::getUniverse(){
        return getObject(0);
    }

    /*! \brief Gets Orders from the server.
    
    This method sends the GetOrder Frame to the server and returns the
    Order Frames.
    \param obid The object to get the orders from.
    \param num The number of orders to get.
    \return List of Orders.
    */
    std::list<Order*> GameLayer::getOrders(uint32_t obid, uint32_t num){
        GetOrder* frame = protocol->getFrameFactory()->createGetOrder();
        frame->setObjectId(obid);
        frame->addOrderRange(0, num);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> reply = protocol->getFrameCodec()->recvFrames(seqnum);
        std::list<Order*> out;
        for(std::list<Frame*>::iterator itcurr = reply.begin(); itcurr != reply.end(); ++itcurr){
            Frame * ob = *itcurr;
            if(ob != NULL && ob->getType() == ft02_Order){
                out.push_back((Order*)ob);
            }else if(ob != NULL){
                logger->debug("Expecting order frames, but got %d instead", ob->getType());
                delete ob;
            }else{
                logger->debug("Expecting order frames, but got NULL");
                
            }
        }
        
        return out;
    }

    /*! \brief Gets an Orders from the server.
    
    This method sends the GetOrder Frame to the server and returns the
    Order Frame.
    \param obid The object to get the orders from.
    \param slot The slot number of the order to get.
    \return The order retreved.
    */
    Order* GameLayer::getOrder(uint32_t obid, uint32_t slot){
        GetOrder* frame = protocol->getFrameFactory()->createGetOrder();
        frame->setObjectId(obid);
        frame->addOrderId(slot);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL){
            if(reply->getType() == ft02_Order){
                
                return dynamic_cast<Order*>(reply);
            }
            delete reply;
        }
        return NULL;
        
    }

    /*! \brief Creates an Order Frame of a given type.
    
    This method creates a new Order Frame, sets the protocol version and
    sets up the order for the given type, including parameters.
    \param type The type number for the order type.
    \return The new Order.
    */
    Order* GameLayer::createOrderFrame(int type){
        return protocol->getFrameBuilder()->buildOrder(type);
    }

    /*! \brief Inserts an Order into the objects order queue.
    
    \param frame The Order to insert.
    \returns True if successful, false otherwise.
    */
    bool GameLayer::insertOrder(Order* frame){
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL){
            if(reply->getType() == ft02_OK){
                
                delete reply;
                
                return true;
            }else{
                logger->debug("Expected ok frame, got %d", reply->getType());
            }
            delete reply;
        }else{
            logger->debug("Expected ok frame, got NULL");
        }
        return false;
    }

    /*! \brief Replaces a current Order with a new one.
    
    First inserts the new Order, then removes the old one.
    Can fail with or without the new order in the order queue.
    \param frame The Order that will replace the current one.
    \return True if successful, false otherwise.
    */
    bool GameLayer::replaceOrder(Order* frame){
        if(frame->getSlot() >= 0 && insertOrder(frame)){
            if(removeOrder(frame->getObjectId(), frame->getSlot() + 1) == 1){
                return true;
            }
        }
        return false;
    }

    /*! \brief Probes an Order.
    
    Sends the order to the server to be probed, returning the order that would have been added
    to the object.
    \param frame The Order to probe.
    \return The reply Order with read-only fields filled.
    */
    Order* GameLayer::probeOrder(Order* frame){
        ProbeOrder * fr = protocol->getFrameFactory()->createProbeOrder();
        fr->copyFromOrder(frame);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
        delete fr;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply == NULL || reply->getType() != ft02_Order){
            logger->error("The returned frame isn't an order");
        }
        
        return static_cast<Order*>(reply);
        
    }

    /*! \brief Removes an Order from the server.
    
    Sends the RemoveOrder frame to the server and receives reply.
    \param obid The Object to remove the order from.
    \param slot The slot that should have it's order removed.
    \return True if sucessful, false otherwise.
    */
    bool GameLayer::removeOrder(uint32_t obid, uint32_t slot){
        RemoveOrder* ro = protocol->getFrameFactory()->createRemoveOrder();
        ro->setObjectId(obid);
        ro->removeOrderId(slot);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(ro);
    
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL){
            if(reply->getType() == ft02_OK){
                
                delete reply;
                
                return true;
            }else{
                logger->debug("Expected ok frame, got %d", reply->getType());
            }
            delete reply;
        }else{
            logger->debug("Expected ok frame, got NULL");
        }
        return false;
    }

    /*! \brief Gets boardids from the server.
        
        Gets the complete list of Board ids.
    \return The set of board id.
    */
    std::set<uint32_t> GameLayer::getBoardIds(){
        return boardcache->getBoardIds();
    }

    /*! \brief Gets a Board from the server.
    
    Sends the GetBoard Frame and gets the Board back from the server.
    \param boardid The Board id for the board to get from the server.
    \return The Board, or NULL if error.
    */
    Board* GameLayer::getBoard(uint32_t boardid){
        return boardcache->getBoard(boardid);
    }

    /*! \brief Gets the logged in player's personal Board.
    
    A little easier and quicker than FrameCodec::getBoards.
    \return The Board object for the Player's Board.
    */
    Board* GameLayer::getPersonalBoard(){
        return getBoard(0);
    }

    /*! \brief Gets Messages from the server.
    
    Sends the GetMessage Frame and receives the Message frames.
    \param boardid The board ID of the board to get the messages from.
    \param num The number of messages to get.
    \return List of Messages.
    */
    std::list<Message*> GameLayer::getMessages(uint32_t boardid, uint32_t num){
        std::list<Message*> out;
        GetMessage* frame = protocol->getFrameFactory()->createGetMessage();
        frame->setBoard(boardid);
        frame->addMessageRange(0, num);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        for(std::list<Frame*>::iterator itcurr = replies.begin(); itcurr != replies.end(); ++itcurr){
            Frame * ob = *itcurr;
            if(ob != NULL && ob->getType() == ft02_Message){
                out.push_back((Message*)ob);
            }else if(ob != NULL){
                logger->debug("Expecting message frames, but got %d instead", ob->getType());
            }else{
                logger->debug("Expecting message frames, but got NULL");
            }
        }
    
        return out;
    
    }

    /*! \brief Creates a Message object.
    \return A new message object.
    */
    Message* GameLayer::createMessage(){
        return protocol->getFrameFactory()->createMessage();
    }

    /*! \brief Gets a Message from the server.
    
    Sends the GetMessage Frame and receives the Message frame.
    \param boardid The board ID of the board to get the message from.
    \param slot The slot number of the message to get.
    \return The Message.
    */
    Message* GameLayer::getMessage(uint32_t boardid, uint32_t slot){
        GetMessage* frame = protocol->getFrameFactory()->createGetMessage();
        frame->setBoard(boardid);
        frame->addMessageId(slot);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);

        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL){
            if(reply->getType() == ft02_Message){
                
                return dynamic_cast<Message*>(reply);
            }
            delete reply;
        }
        return NULL;
    }

    /*! \brief Posts a Message to the server.
    
    Sends the Message Frame to the server.
    \param frame The Message to post.
    \return True if successful, false otherwise.
    */
    bool GameLayer::postMessage(Message* frame){
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL){
            if(reply->getType() == ft02_OK){
                
                delete reply;
                
                return true;
            }
            delete reply;
        }
        return false;
    }

    /*! \brief Removes messages from the server.
    
    Sends the RemoveMessage frame and receives the replies.
    \param boardid The board id of the board to remove the message from.
    \param slot The slot to remove the message from.
    \return True if message is removed, false otherwise.
    */
    bool GameLayer::removeMessage(uint32_t boardid, uint32_t slot){
        RemoveMessage* frame = protocol->getFrameFactory()->createRemoveMessage();
        frame->setBoard(boardid);
        frame->removeMessageId(slot);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }

        if(reply != NULL && reply->getType() == ft02_OK){
            delete reply;
            return true;
            
        }
        delete reply;
        
        return false;
    }


    /*! \brief Gets a Resource Description from the server.
    
    Gets and returns a Resource Description from the server..
    \param restype The type of resource to get the description for.
    \return The ResourceDescription..
    */
    ResourceDescription* GameLayer::getResourceDescription(uint32_t restype){
        return resourcecache->getResourceDescription(restype);
    }

    /*! \brief Gets a player from the server.
    
    Gets a player from the server and returns the Player.
    \param playerid The player id of the player to get.
    \return The Player.
    */
    Player* GameLayer::getPlayer(uint32_t playerid){
        return playercache->getPlayer(playerid);
    }

    /*! \brief Gets Category ids from the server.
    
    Gets the complete list of Category ids.
    \return The set of category id.
    */
    std::set<uint32_t> GameLayer::getCategoryIds(){
        return categorycache->getCategoryIds();
    }

    /*! \brief Gets a category from the server.
    
    Gets a category from the server and returns it.
    \param catid The Category id of the category to get.
    \return The Category.
    */
    Category* GameLayer::getCategory(uint32_t catid){
        return categorycache->getCategory(catid);
    }

    /*! \brief Creates a Category object.
    \return A new category object.
    */
    Category* GameLayer::createCategory(){
        return protocol->getFrameFactory()->createCategory();
    }

    /*! \brief Adds a Category to the server.
    
    Sends the Category Frame to the server.
    \param cat The Category to add.
    \return True if successful, false otherwise.
    */
    bool GameLayer::addCategory(Category* cat){
        return categorycache->addCategory(cat);
    }

    /*! \brief Removes a category from the server.
    
    Sends the RemoveCategory frame and receives the reply.
    \param catid The Category Id to remove.
    \return True if sucessful, false otherwise.
  */
    bool GameLayer::removeCategory(uint32_t catid){
        return categorycache->removeCategory(catid);
    }
    

    /*! \brief Gets designids from the server.
    
    Gets the complete list of Design ids.
    \return The set of design id.
    */
    std::set<uint32_t> GameLayer::getDesignIds(){
        return designcache->getDesignIds();
    }

    /*! \brief Gets a design from the server.
    
    Gets a design from the server and returns the Design.
    \param designid The Design id of the design to get.
    \return The Design.
    */
    Design* GameLayer::getDesign(uint32_t designid){
        return designcache->getDesign(designid);
    }

    /*! \brief Creates a Design object.
    \return A new design object.
    */
    Design* GameLayer::createDesign(){
        return protocol->getFrameFactory()->createDesign();
    }

    /*! \brief Adds a Design to the server.
    
    Sends the Design Frame to the server.
    \param d The Design to add.
    \return True if successful, false otherwise.
    */
    bool GameLayer::addDesign(Design* d){
        return designcache->addDesign(d);
    }

    /*! \brief Modifies a Design on the server.
    
    Sends a ModifyDesign Frame to the server.
    \param d The Design to modify.
    \return True if successful, false otherwise.
    */
    bool GameLayer::modifyDesign(Design* d){
        return designcache->modifyDesign(d);
    }
    
    /*! \brief Removes a design from the server.
    
    Sends the RemoveDesign frame and receives the reply.
    \param designid The Design Id to remove.
    \return True if sucessful, false otherwise.
    */
    bool GameLayer::removeDesign(uint32_t designid){
        return designcache->removeDesign(designid);
    }


    /*! \brief Gets component ids from the server.
    
    Gets the complete list of Component ids.
    \return The set of component id.
    */
    std::set<uint32_t> GameLayer::getComponentIds(){
        return componentcache->getComponentIds();
    }

    /*! \brief Gets a Component from the server.
    
    Gets a component from the server and returns the Component.
    \param compid The Component id of the component to get.
    \return The Component.
    */
    Component* GameLayer::getComponent(uint32_t compid){
        return componentcache->getComponent(compid);
    }


    /*! \brief Gets propertyids from the server.
    
    Gets the complete list of Property ids.
    \return The set of property id.
    */
    std::set<uint32_t> GameLayer::getPropertyIds(){
        return propertycache->getPropertyIds();
    }

    /*! \brief Gets a Property from the server.
    
    Gets a property from the server and returns the Property.
    \param propid The property id of the property to get.
    \return The Property.
    */
    Property* GameLayer::getProperty(uint32_t propid){
        return propertycache->getProperty(propid);
    }


    /*! \brief Gets the time remaining before the end of turn.
    
    Fetches the time remaining till the end of turn from the server.
    \returns The time in seconds before the end of turn, or
    -1 if there was an error.
    */
    int GameLayer::getTimeRemaining(){
        GetTime* gt = protocol->getFrameFactory()->createGetTimeRemaining();
        
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(gt);
        delete gt;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL && reply->getType() == ft02_Time_Remaining){
            int time = ((TimeRemaining*)reply)->getTimeRemaining();
            delete reply;
            if(statuslistener != NULL)
                statuslistener->timeToEot(time);
            return time;
        }
        if(reply != NULL)
            delete reply;
        return -1;
    }


}
