/*  GameLayer class
 *
 *  Copyright (C) 2005  Lee Begg and the Thousand Parsec Project
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
#include "silentlogger.h"
#include "framecodec.h"
#include "framebuilder.h"
#include "tcpsocket.h"

// Frame Types

#include "okframe.h"
#include "failframe.h"
#include "sequence.h"
#include "connect.h"
#include "login.h"
#include "getobjectbyid.h"
#include "getobjects.h"
#include "object.h"
#include "getboard.h"
#include "board.h"
#include "getmessage.h"
#include "message.h"
#include "removemessage.h"
#include "gettime.h"
#include "timeremaining.h"
#include "asyncframelistener.h"
#include "order.h"
#include "getorder.h"
#include "removeorder.h"
#include "orderdesc.h"
#include "getorderdesc.h"
#include "featuresframe.h"
#include "redirect.h"
#include "getobjectidslist.h"
#include "objectidslist.h"
#include "getboardidslist.h"
#include "boardidslist.h"
#include "getcategoryidslist.h"
#include "categoryidslist.h"
#include "getdesignidslist.h"
#include "designidslist.h"
#include "getcomponentidslist.h"
#include "componentidslist.h"
#include "getpropertyidslist.h"
#include "propertyidslist.h"
#include "getresourcedesc.h"
#include "resourcedesc.h"
#include "getplayer.h"
#include "player.h"
#include "getcategory.h"
#include "category.h"
#include "getdesign.h"
#include "design.h"
#include "getcomponent.h"
#include "component.h"
#include "getproperty.h"
#include "property.h"
#include "probeorder.h"
#include "addcategory.h"
#include "removecategory.h"
#include "adddesign.h"
#include "modifydesign.h"
#include "removedesign.h"

#include "gamelayer.h"

namespace TPProto {

    /*! \brief Constructs object and sets up defaults.

    Defaults are
        - The default ProtocolLayer
        - Disconnected state.
        - "Unknown client" for the client string
    */
    GameLayer::GameLayer() : protocol(NULL), logger(NULL), status(gsDisconnected), clientid("Unknown client"),
            serverfeatures(NULL){
        protocol = new ProtocolLayer();
        logger = new SilentLogger();
        sock = NULL;
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

    void GameLayer::setLogger(Logger* nlog){
        logger = nlog;
        protocol->getFrameCodec()->setLogger(nlog);
    }

    GameStatus GameLayer::getStatus(){
        if(sock == NULL || !sock->isConnected())
            status = gsDisconnected;
        return status;
    }

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

        TPSocket *sock;
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
            //TODO
        }else if(type == "http"){
            if(port.empty()){
                port = "80";
            }
            //TODO
        }else if(type == "https"){
            if(port.empty()){
                port = "443";
            }
            //TODO
        }else{
            logger->error("Type of connection to create was not known");
            return false;
        }
        return connect(sock);
    }
        
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
                logger->info("Connected");
                delete reply;
                return true;
            }else if(reply != NULL && reply->getType() == ft03_Redirect){
                //signal we are redirecting 
                //TODO listener
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
                logger->info("Logged in");
                delete reply;
                return true;
            }else{
                logger->warning("Did not log in");
                if(reply != NULL)
                    delete reply;
            }
            
        }
        if(!sock->isConnected())
            status = gsDisconnected;
        return false;
    }

    /*! \brief Disconnects from server.
    
    Closes the underlying TPSocket.
    */
    void GameLayer::disconnect(){
        if(status != gsDisconnected && sock != NULL){
            sock->disconnect();
            logger->info("Disconnected");
        }
        status = gsDisconnected;
    }




    /*! \brief Gets objectids from the server.
    
    Gets the complete list of Object ids.
    \return The set of object id.
    */
    std::set<uint32_t> GameLayer::getObjectIds(){
        std::set<uint32_t> out;
        GetObjectIdsList *frame = protocol->getFrameFactory()->createGetObjectIdsList();
        frame->setCount(10000); // When this code is shifted out, this should be in a loop to get all the items
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
    
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
         if(reply != NULL && reply->getType() == ft03_ObjectIds){
             std::map<uint32_t, uint64_t> ids = static_cast<ObjectIdsList*>(reply)->getIds();
             for(std::map<uint32_t, uint64_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
                 out.insert(itcurr->first);
             }
        }else{
            logger->debug("Expecting objectidlist frame, but got %d instead", reply->getType());
        }

        return out;
    }

    /*! \brief Gets an object from the server.
    
    Gets an object from the server and returns the Object.
    \param obid The Object id of the object to get.
    \return The Object.
    */
    Object* GameLayer::getObject(uint32_t obid){
        GetObjectById * fr = protocol->getFrameFactory()->createGetObjectById();
        fr->addObjectID(obid);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
        delete fr;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply == NULL || reply->getType() != ft02_Object){
            logger->error("The returned frame isn't an object");
        }
        
        return static_cast<Object*>(reply);
        
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
        std::set<uint32_t> out;
        GetBoardIdsList *frame = protocol->getFrameFactory()->createGetBoardIdsList();
        frame->setCount(10000); // When this code is shifted out, this should be in a loop to get all the items
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply != NULL && reply->getType() == ft03_BoardIds){
            std::map<uint32_t, uint64_t> ids = static_cast<BoardIdsList*>(reply)->getIds();
            for(std::map<uint32_t, uint64_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
                out.insert(itcurr->first);
            }
        }else{
            logger->debug("Expecting boardidlist frame, but got %d instead", reply->getType());
        }
        
            return out;
    }

    /*! \brief Gets a Board from the server.
    
    Sends the GetBoard Frame and gets the Board back from the server.
    \param frame The GetBoard frame to send to the server.
    \return A map of BoardId and Board pairs.
    */
    Board* GameLayer::getBoard(uint32_t boardid){
        GetBoard* frame = protocol->getFrameFactory()->createGetBoard();
        frame->addBoardId(boardid);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL && reply->getType() == ft02_Board){
            return static_cast<Board*>(reply);
        }else{
            logger->debug("Expecting Board frame, but got %d instead", reply->getType());
            delete reply;
        }

        return NULL;
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
\param frame The RemoveMessage frame to send.
\return The number of Messages removed.
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
        GetResourceDescription * fr = protocol->getFrameFactory()->createGetResourceDescription();
        fr->addResourceType(restype);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
        delete fr;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply == NULL || reply->getType() != ft02_ResDesc){
            logger->error("The returned frame isn't a resource description");
        }
        
        return static_cast<ResourceDescription*>(reply);
        
    }

    /*! \brief Gets a player from the server.
    
    Gets a player from the server and returns the Player.
    \param playerid The player id of the player to get.
    \return The Player.
    */
    Player* GameLayer::getPlayer(uint32_t playerid){
        GetPlayer * fr = protocol->getFrameFactory()->createGetPlayer();
        fr->addPlayerId(playerid);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
        delete fr;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
    
        if(reply == NULL || reply->getType() != ft03_Player){
            logger->error("The returned frame isn't a player");
        }
    
        return static_cast<Player*>(reply);
        
    }

    /*! \brief Gets Category ids from the server.
    
    Gets the complete list of Category ids.
    \return The set of category id.
    */
    std::set<uint32_t> GameLayer::getCategoryIds(){
        std::set<uint32_t> out;
        GetCategoryIdsList *frame = protocol->getFrameFactory()->createGetCategoryIdsList();
        frame->setCount(10000); // When this code is shifted out, this should be in a loop to get all the items
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
    
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply != NULL && reply->getType() == ft03_CategoryIds){
            std::map<uint32_t, uint64_t> ids = static_cast<CategoryIdsList*>(reply)->getIds();
            for(std::map<uint32_t, uint64_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
                out.insert(itcurr->first);
            }
        }else{
            logger->debug("Expecting categoryidlist frame, but got %d instead", reply->getType());
        }
        
        return out;
    }

    /*! \brief Gets a category from the server.
    
    Gets a category from the server and returns it.
    \param catid The Category id of the category to get.
    \return The Category.
    */
    Category* GameLayer::getCategory(uint32_t catid){
        GetCategory * fr = protocol->getFrameFactory()->createGetCategory();
        fr->addCategoryId(catid);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
        delete fr;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply == NULL || reply->getType() != ft03_Category){
            logger->error("The returned frame isn't a category");
        }
        
        return static_cast<Category*>(reply);
        
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
        AddCategory* frame = protocol->getFrameFactory()->createAddCategory();
        frame->setName(cat->getName());
        frame->setDescription(cat->getDescription());
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
            }
            delete reply;
        }
        return false;
    }

    /*! \brief Removes a category from the server.
    
    Sends the RemoveCategory frame and receives the reply.
    \param catid The Category Id to remove.
    \return True if sucessful, false otherwise.
  */
    bool GameLayer::removeCategory(uint32_t catid){
        RemoveCategory* frame = protocol->getFrameFactory()->createRemoveCategory();
        frame->removeCategoryId(catid);
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
    

    /*! \brief Gets designids from the server.
    
    Gets the complete list of Design ids.
    \return The set of design id.
    */
    std::set<uint32_t> GameLayer::getDesignIds(){
        std::set<uint32_t> out;
        GetDesignIdsList *frame = protocol->getFrameFactory()->createGetDesignIdsList();
        frame->setCount(10000); // When this code is shifted out, this should be in a loop to get all the items
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply != NULL && reply->getType() == ft03_DesignIds){
            std::map<uint32_t, uint64_t> ids = static_cast<DesignIdsList*>(reply)->getIds();
            for(std::map<uint32_t, uint64_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
                out.insert(itcurr->first);
            }
        }else{
            logger->debug("Expecting designidlist frame, but got %d instead", reply->getType());
        }
        
        return out;
    }

    /*! \brief Gets a design from the server.
    
    Gets a design from the server and returns the Design.
    \param designid The Design id of the design to get.
    \return The Design.
    */
    Design* GameLayer::getDesign(uint32_t designid){
        GetDesign * fr = protocol->getFrameFactory()->createGetDesign();
        fr->addDesignId(designid);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
        delete fr;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply == NULL || reply->getType() != ft03_Design){
            logger->error("The returned frame isn't a design");
        }
        
        return static_cast<Design*>(reply);
        
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
        AddDesign* frame = protocol->getFrameFactory()->createAddDesign();
        frame->copyFromDesign(d);
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
            }
            delete reply;
        }
        return false;
    }

    /*! \brief Modifies a Design on the server.
    
    Sends a ModifyDesign Frame to the server.
    \param d The Design to modify.
    \return True if successful, false otherwise.
    */
    bool GameLayer::modifyDesign(Design* d){
        ModifyDesign* frame = protocol->getFrameFactory()->createModifyDesign();
        frame->copyFromDesign(d);
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
            }
            delete reply;
        }
        return false;
    }
    
    /*! \brief Removes a design from the server.
    
    Sends the RemoveDesign frame and receives the reply.
    \param designid The Design Id to remove.
    \return True if sucessful, false otherwise.
    */
    bool GameLayer::removeDesign(uint32_t designid){
        RemoveDesign* frame = protocol->getFrameFactory()->createRemoveDesign();
        frame->removeDesignId(designid);
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


    /*! \brief Gets component ids from the server.
    
    Gets the complete list of Component ids.
    \return The set of component id.
    */
    std::set<uint32_t> GameLayer::getComponentIds(){
        std::set<uint32_t> out;
        GetComponentIdsList *frame = protocol->getFrameFactory()->createGetComponentIdsList();
        frame->setCount(10000); // When this code is shifted out, this should be in a loop to get all the items
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply != NULL && reply->getType() == ft03_ComponentIds){
            std::map<uint32_t, uint64_t> ids = static_cast<ComponentIdsList*>(reply)->getIds();
            for(std::map<uint32_t, uint64_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
                out.insert(itcurr->first);
            }
        }else{
            logger->debug("Expecting componentidlist frame, but got %d instead", reply->getType());
        }
        
        return out;
    }

    /*! \brief Gets a Component from the server.
    
    Gets a component from the server and returns the Component.
    \param compid The Component id of the component to get.
    \return The Component.
    */
    Component* GameLayer::getComponent(uint32_t compid){
        GetComponent * fr = protocol->getFrameFactory()->createGetComponent();
        fr->addComponentId(compid);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
        delete fr;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply == NULL || reply->getType() != ft03_Component){
            logger->error("The returned frame isn't a component");
        }
        
        return static_cast<Component*>(reply);
        
    }


    /*! \brief Gets propertyids from the server.
    
    Gets the complete list of Property ids.
    \return The set of property id.
    */
    std::set<uint32_t> GameLayer::getPropertyIds(){
        std::set<uint32_t> out;
        GetPropertyIdsList *frame = protocol->getFrameFactory()->createGetPropertyIdsList();
        frame->setCount(10000); // When this code is shifted out, this should be in a loop to get all the items
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply != NULL && reply->getType() == ft03_PropertyIds){
            std::map<uint32_t, uint64_t> ids = static_cast<PropertyIdsList*>(reply)->getIds();
            for(std::map<uint32_t, uint64_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
                out.insert(itcurr->first);
            }
        }else{
            logger->debug("Expecting objectidlist frame, but got %d instead", reply->getType());
        }
        
        return out;
    }

    /*! \brief Gets a Property from the server.
    
    Gets a property from the server and returns the Property.
    \param propid The property id of the property to get.
    \return The Property.
    */
    Property* GameLayer::getProperty(uint32_t propid){
        GetProperty * fr = protocol->getFrameFactory()->createGetProperty();
        fr->addPropertyId(propid);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
        delete fr;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply == NULL || reply->getType() != ft03_Property){
            logger->error("The returned frame isn't a property");
        }
        
        return static_cast<Property*>(reply);
        
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
            return time;
        }
        if(reply != NULL)
            delete reply;
        return -1;
    }


}