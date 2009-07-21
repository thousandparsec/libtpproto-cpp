/*  GameLayer class
 *
 *  Copyright (C) 2005-2006, 2008  Lee Begg and the Thousand Parsec Project
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
#include <boost/bind.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef VERSION
#define VERSION "0.0.0"
#endif

#include "tpsocket.h"
#include "framefactory.h"
#include "protocollayer.h"
#include "eventloop.h"
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
#include "objectdesccache.h"
#include "objectcache.h"
#include"orderdesccache.h"
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
#include "createaccount.h"
#include "login.h"
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
#include "getfeatures.h"
#include "redirect.h"
#include "probeorder.h"
#include "finished.h"
#include "getgameinfo.h"
#include "gameinfo.h"

#include "gamelayer.h"

namespace TPProto {

    class LogMessage;

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
                if(layer->status == gsLoggedIn){
                    layer->status = gsEOTInProgress;
                }
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

        // LogMessage frames are not used by game clients
        void recvLogMessage(LogMessage* lmf){}
        
    private:
        GameLayer* layer;
    };

    /*! \brief Constructs object and sets up defaults.

    Defaults are
        - The default ProtocolLayer
        - Disconnected state.
        - "Unknown client" for the client string
    */
    GameLayer::GameLayer() : protocol(NULL), eventloop(NULL), logger(NULL), statuslistener(NULL), status(gsDisconnected),
            clientid("Unknown client"), serverfeatures(NULL), asyncframes(new GameLayerAsyncFrameListener()),
            objectdesccache(new ObjectDescCache()),
            objectcache(new ObjectCache()), orderdesccache(new OrderDescCache()), 
            playercache(new PlayerCache()), boardcache(new BoardCache()),
            resourcecache(new ResourceCache()), categorycache(new CategoryCache()),
            designcache(new DesignCache()), componentcache(new ComponentCache()),
            propertycache(new PropertyCache()){
        protocol = new ProtocolLayer();
        logger = new SilentLogger();
        sock = NULL;
        asyncframes->setGameLayer(this);
        protocol->getFrameCodec()->setAsyncFrameListener(asyncframes);
        objectdesccache->setProtocolLayer(protocol);
        objectcache->setProtocolLayer(protocol);
        orderdesccache->setProtocolLayer(protocol);
        playercache->setProtocolLayer(protocol);
        boardcache->setProtocolLayer(protocol);
        resourcecache->setProtocolLayer(protocol);
        categorycache->setProtocolLayer(protocol);
        designcache->setProtocolLayer(protocol);
        componentcache->setProtocolLayer(protocol);
        propertycache->setProtocolLayer(protocol);
        protocol->getFrameBuilder()->setOrderDescCache(orderdesccache);
        protocol->getFrameBuilder()->setObjectDescCache(objectdesccache);
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
        delete objectdesccache;
        delete objectcache;
        delete orderdesccache;
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
        if((sock == NULL || !sock->isConnected()) && status != gsDisconnected){
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
        objectdesccache->setCacheMethod(prototype->clone());
        objectcache->setCacheMethod(prototype->clone());
        orderdesccache->setCacheMethod(prototype->clone());
        boardcache->setCacheMethod(prototype->clone());
        playercache->setCacheMethod(prototype->clone());
        resourcecache->setCacheMethod(prototype->clone());
        categorycache->setCacheMethod(prototype->clone());
        designcache->setCacheMethod(prototype->clone());
        componentcache->setCacheMethod(prototype->clone());
        propertycache->setCacheMethod(prototype->clone());
    }
    
    /*! \brief Sets the EventLoop abstraction to use.
    Sets which implementation of EventLoop to use.
    
    Must be set before connecting to a server.
    \param el The implementation of the EventLoop to use.
    */
    void GameLayer::setEventLoop(EventLoop* el){
        eventloop = el;
        protocol->getFrameCodec()->setEventLoop(eventloop);
    }

    /*! \brief Gets the ProtocolLayer being used.
    This could be used to do low level calls to the protocol itself, or 
    more importantly, set the FrameFactory, FrameBuilder and/or FrameCodec
    classes in the ProtocolLayer.
    \return The pointer to the ProtocolLayer.
    */
    ProtocolLayer* GameLayer::getProtocolLayer() const{
      return protocol;
    }

    /*! \brief Connects to the given address url
    This method connects to the server given as the address. The types of url
    supported are tp, tps, https and http. Tps and https depend on TLS being enabled.
    \param address The URL to connect to.
    \return True if connection in progress, false otherwise.
    */
    bool GameLayer::connect(const std::string& address){
        if(status != gsDisconnected){
            logger->warning("Already connected, ignoring connection attempt");
            return false;
        }
        if(eventloop == NULL){
            logger->error("Event Loop not set, not connecting");
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
    \return True if connection in progress, false otherwise.
    */
    bool GameLayer::connect(TPSocket* nsock){
        if(status != gsDisconnected){
            logger->warning("Already connected, ignoring connection attempt");
            return false;
        }
        if(eventloop == NULL){
            logger->error("Event Loop not set, not connecting");
            return false;
        }
        sock = nsock;
        sock->setConnection(protocol->getFrameCodec());
        if(sock->connect()){
            eventloop->listenForSocketRead(sock);
            logger->debug("Connection opened");
            status = gsConnecting;
            Connect * cf = protocol->getFrameFactory()->createConnect();
            cf->setClientString(std::string("libtpproto-cpp/") + VERSION + " " + clientid);
            protocol->getFrameCodec()->sendFrame(boost::shared_ptr<Connect>(cf), boost::bind(&GameLayer::connectCallback, this, _1));
            return true;
        }else{
            logger->error("Could not open socket to server");
        }
        return false;
    }
    
    /*! \brief Creates an account on the server.
    
    Sends a AccountCreate Frame to the server and waits for a reply.
    \param user The username to use.
    \param password The password for the account.
    \param email The user's email address.
    \param comment A comment to send.
    \return True if successful, false otherwise.
    */
    bool GameLayer::createAccount(const std::string &user, const std::string &password, const std::string &email, const std::string &comment){
      if(status == gsConnected && sock->isConnected()){
          if(serverfeatures != NULL && serverfeatures->supportsAccountCreation()){
            AccountCreate * account = protocol->getFrameFactory()->createAccountCreate();
            account->setUser(user);
            account->setPass(password);
            account->setEmail(email);
            account->setComment(comment);
            protocol->getFrameCodec()->sendFrame(boost::shared_ptr<Frame>(account), boost::bind(&GameLayer::accountCreateCallback, this, _1));
          }else{
              logger->error("Account creation not supported on the server");
              return false;
          }
          
          return true;

      }
      if(!sock->isConnected()){
          status = gsDisconnected;
          if(statuslistener != NULL)
              statuslistener->disconnected();
      }
      return false;
    }

    /*! \brief Logs in to the server.
    
    Sends a Login Frame to the server and waits for a reply.
    \param username The username to connect as.
    \param password The password of the account of the username.
    \return True if successfully started, false otherwise.
    */
    bool GameLayer::login(const std::string &username, const std::string &password){
        if(status == gsConnected && sock->isConnected()){
            Login * login = protocol->getFrameFactory()->createLogin();
            login->setUser(username);
            login->setPass(password);
           protocol->getFrameCodec()->sendFrame(boost::shared_ptr<Login>(login), boost::bind(&GameLayer::loginCallback, this, _1));
            return true;
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
             status = gsDisconnected;
            if(statuslistener != NULL)
                statuslistener->disconnected();
        }
    }

    /*! \brief Tells all the caches to update.
    Called automatically after logged in, and after EOT has finished.
    Call if you want the caches to be updated.
    */
    void GameLayer::updateCaches(){
        objectdesccache->update();
        objectcache->update();
        orderdesccache->update();
        boardcache->update();
        playercache->update();
        resourcecache->update();
        categorycache->update();
        designcache->update();
        componentcache->update();
        propertycache->update();
    }

    
    /*! \brief Gets the ObjectDescCache.
    \return The ObjectDescCache.
    */
    ObjectDescCache* GameLayer::getObjectDescCache() const{
        return objectdesccache;
    }

    /*! \brief Gets the ObjectCache.
    
    \return The ObjectCache.
    */
    ObjectCache* GameLayer::getObjectCache() const{
        return objectcache;
    }

    /*! \brief Gets the OrderDescCache.
    \return The OrderDescCache.
    */
    OrderDescCache* GameLayer::getOrderDescCache() const{
        return orderdesccache;
    }
    
    /*! \brief Gets Orders from the server.
    
    This method sends the GetOrder Frame to the server and returns the
    Order Frames.
    \param obid The object to get the orders from.
    \param num The number of orders to get.
    \return List of Orders.
    */
    std::list<Order*> GameLayer::getOrders(uint32_t obid, uint32_t num){
//         GetOrder* frame = protocol->getFrameFactory()->createGetOrder();
//         frame->setObjectId(obid);
//         frame->addOrderRange(0, num);
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
//         delete frame;
//         std::list<Frame*> reply = protocol->getFrameCodec()->recvFrames(seqnum);
        std::list<Order*> out;
//         for(std::list<Frame*>::iterator itcurr = reply.begin(); itcurr != reply.end(); ++itcurr){
//             Frame * ob = *itcurr;
//             if(ob != NULL && ob->getType() == ft02_Order){
//                 out.push_back((Order*)ob);
//             }else if(ob != NULL){
//                 logger->debug("Expecting order frames, but got %d instead", ob->getType());
//                 delete ob;
//             }else{
//                 logger->debug("Expecting order frames, but got NULL");
//                 
//             }
//         }
//         
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
//         GetOrder* frame = protocol->getFrameFactory()->createGetOrder();
//         frame->setObjectId(obid);
//         frame->addOrderId(slot);
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
//         delete frame;
//         std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
//         
//         Frame * reply = NULL;
//         if(replies.size() >= 1){
//             reply = replies.front();
//         }
//         if(reply != NULL){
//             if(reply->getType() == ft02_Order){
//                 
//                 return dynamic_cast<Order*>(reply);
//             }
//             delete reply;
//         }
        return NULL;
        
    }

    /*! \brief Creates an Order Frame of a given type.
    
    This method creates a new Order Frame, sets the protocol version and
    sets up the order for the given type, including parameters.
    \param type The type number for the order type.
    \return The new Order.
    */
    Order* GameLayer::createOrderFrame(int type){
        //return protocol->getFrameBuilder()->buildOrder(type);
        return NULL;
    }

    /*! \brief Inserts an Order into the objects order queue.
    
    \param frame The Order to insert.
    \returns True if successful, false otherwise.
    */
    bool GameLayer::insertOrder(Order* frame){
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
//         std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
//         Frame * reply = NULL;
//         if(replies.size() >= 1){
//             reply = replies.front();
//         }
//         if(reply != NULL){
//             if(reply->getType() == ft02_OK){
//                 
//                 delete reply;
//                 
//                 return true;
//             }else{
//                 logger->debug("Expected ok frame, got %d", reply->getType());
//             }
//             delete reply;
//         }else{
//             logger->debug("Expected ok frame, got NULL");
//         }
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
//         ProbeOrder * fr = protocol->getFrameFactory()->createProbeOrder();
//         fr->copyFromOrder(frame);
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(fr);
//         delete fr;
//         std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
//         Frame * reply = NULL;
//         if(replies.size() >= 1){
//             reply = replies.front();
//         }
//         
//         if(reply == NULL || reply->getType() != ft02_Order){
//             logger->error("The returned frame isn't an order");
//         }
//         
//         return static_cast<Order*>(reply);
        return NULL;
        
    }

    /*! \brief Removes an Order from the server.
    
    Sends the RemoveOrder frame to the server and receives reply.
    \param obid The Object to remove the order from.
    \param slot The slot that should have it's order removed.
    \return True if sucessful, false otherwise.
    */
    bool GameLayer::removeOrder(uint32_t obid, uint32_t slot){
//         RemoveOrder* ro = protocol->getFrameFactory()->createRemoveOrder();
//         ro->setObjectId(obid);
//         ro->removeOrderId(slot);
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(ro);
//     
//         std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
//         Frame * reply = NULL;
//         if(replies.size() >= 1){
//             reply = replies.front();
//         }
//         if(reply != NULL){
//             if(reply->getType() == ft02_OK){
//                 
//                 delete reply;
//                 
//                 return true;
//             }else{
//                 logger->debug("Expected ok frame, got %d", reply->getType());
//             }
//             delete reply;
//         }else{
//             logger->debug("Expected ok frame, got NULL");
//         }
        return false;
    }

    /*! \brief Gets the BoardCache.
        
    \return The BoardCache.
    */
    BoardCache* GameLayer::getBoardCache() const{
        return boardcache;
    }

    /*! \brief Gets Messages from the server.
    
    Sends the GetMessage Frame and receives the Message frames.
    \param boardid The board ID of the board to get the messages from.
    \param num The number of messages to get.
    \return List of Messages.
    */
    std::list<Message*> GameLayer::getMessages(uint32_t boardid, uint32_t num){
        std::list<Message*> out;
//         GetMessage* frame = protocol->getFrameFactory()->createGetMessage();
//         frame->setBoard(boardid);
//         frame->addMessageRange(0, num);
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
//         delete frame;
//         std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
//         for(std::list<Frame*>::iterator itcurr = replies.begin(); itcurr != replies.end(); ++itcurr){
//             Frame * ob = *itcurr;
//             if(ob != NULL && ob->getType() == ft02_Message){
//                 out.push_back((Message*)ob);
//             }else if(ob != NULL){
//                 logger->debug("Expecting message frames, but got %d instead", ob->getType());
//             }else{
//                 logger->debug("Expecting message frames, but got NULL");
//             }
//         }
//     
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
//         GetMessage* frame = protocol->getFrameFactory()->createGetMessage();
//         frame->setBoard(boardid);
//         frame->addMessageId(slot);
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
//         delete frame;
//         std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
// 
//         Frame * reply = NULL;
//         if(replies.size() >= 1){
//             reply = replies.front();
//         }
//         if(reply != NULL){
//             if(reply->getType() == ft02_Message){
//                 
//                 return dynamic_cast<Message*>(reply);
//             }
//             delete reply;
//         }
        return NULL;
    }

    /*! \brief Posts a Message to the server.
    
    Sends the Message Frame to the server.
    \param frame The Message to post.
    \return True if successful, false otherwise.
    */
    bool GameLayer::postMessage(Message* frame){
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
//         delete frame;
//         std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
//         Frame * reply = NULL;
//         if(replies.size() >= 1){
//             reply = replies.front();
//         }
//         if(reply != NULL){
//             if(reply->getType() == ft02_OK){
//                 
//                 delete reply;
//                 
//                 return true;
//             }
//             delete reply;
//         }
        return false;
    }

    /*! \brief Removes messages from the server.
    
    Sends the RemoveMessage frame and receives the replies.
    \param boardid The board id of the board to remove the message from.
    \param slot The slot to remove the message from.
    \return True if message is removed, false otherwise.
    */
    bool GameLayer::removeMessage(uint32_t boardid, uint32_t slot){
//         RemoveMessage* frame = protocol->getFrameFactory()->createRemoveMessage();
//         frame->setBoard(boardid);
//         frame->removeMessageId(slot);
//         uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
//         delete frame;
//         std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
//         
//         Frame * reply = NULL;
//         if(replies.size() >= 1){
//             reply = replies.front();
//         }
// 
//         if(reply != NULL && reply->getType() == ft02_OK){
//             delete reply;
//             return true;
//             
//         }
//         delete reply;
//         
        return false;
    }


    /*! \brief Gets the ResourceCache.
    
    \return The ResourceCache.
    */
    ResourceCache* GameLayer::getResourceCache() const{
        return resourcecache;
    }

    /*! \brief Gets the PlayerCache.
    
    \return The PlayerCache.
    */
    PlayerCache* GameLayer::getPlayerCache() const{
        return playercache;
    }
    
    /*! \brief Gets the CategoryCache.
    
    \return The CategoryCache.
    */
    CategoryCache* GameLayer::getCategoryCache() const{
        return categorycache;
    }

    /*! \brief Gets the DesignCache.
    
    \return The DesignCache.
    */
    DesignCache* GameLayer::getDesignCache() const{
        return designcache;
    }

    /*! \brief Gets the ComponentCache.
    
    \return The ComponentCache.
    */
    ComponentCache* GameLayer::getComponentCache() const{
        return componentcache;
    }

    /*! \brief Gets the PropertyCache.
    
    \return The PropertyCache.
    */
    PropertyCache* GameLayer::getPropertyCache() const{
        return propertycache;
    }


    /*! \brief Gets the time remaining before the end of turn.
    
    Fetches the time remaining till the end of turn from the server.
    The actual value is returned through the GameStatusListener::timeToEoT().
    */
    void GameLayer::getTimeRemaining(){
        GetTime* gt = protocol->getFrameFactory()->createGetTimeRemaining();
        
        protocol->getFrameCodec()->sendFrame(boost::shared_ptr<GetTime>(gt), boost::bind(&GameLayer::timeRemainingCallback, this, _1));
    }

    /*! \brief Tells the server that the player has finished their turn.
    
    Sends a FinishedFrame to the server.
    */
    void GameLayer::finishedTurn(){
        if(protocol->getFrameFactory()->getProtocolVersion() > 3){
            boost::shared_ptr<FinishedFrame> ft(protocol->getFrameFactory()->createFinished());
            if(ft){
                protocol->getFrameCodec()->sendFrame(ft, boost::bind(&GameLayer::finishedTurnCallback, this, _1));
            }
        }
    }

    void GameLayer::connectCallback(Frame* frame){
        if(frame->getType() == ft02_OK){
            delete frame;
            status = gsConnected;
            logger->info("Connected");
            //get features
            GetFeatures * gf = protocol->getFrameFactory()->createGetFeatures();
            protocol->getFrameCodec()->sendFrame(boost::shared_ptr<GetFeatures>(gf), boost::bind(&GameLayer::featureCallback, this, _1));
            //if tp04, get game info
            boost::shared_ptr<GetGameInfo> ggi(protocol->getFrameFactory()->createGetGameInfo());
            if(ggi){
                protocol->getFrameCodec()->sendFrame(ggi, boost::bind(&GameLayer::gameInfoCallback, this, _1));
            }
            
            
            if(statuslistener != NULL)
                statuslistener->connected();
        }else if(frame->getType() == ft03_Redirect){
            status = gsDisconnected;
            sock->disconnect();
            if(statuslistener == NULL || (statuslistener != NULL && statuslistener->redirected(static_cast<Redirect*>(frame)->getUrl()))){
                connect(static_cast<Redirect*>(frame)->getUrl());
            }
            delete frame;
        }else{
            status = gsDisconnected;
            logger->error("Could not connect");
            //TODO: check why, maybe wrong protocol version?
            sock->disconnect();
            if(statuslistener != NULL)
                statuslistener->disconnected();
            delete frame;
        }
    }
    
    void GameLayer::featureCallback(Frame* frame){
        if(frame->getType() == ft03_Features){
            logger->debug("New features frame received");
            if(serverfeatures != NULL)
                delete serverfeatures;
            serverfeatures = (Features*)frame;
        }else{
            delete frame;
        }
    }
    
    void GameLayer::loginCallback(Frame* frame){
        if(frame->getType() == ft02_OK){
            delete frame;
            status = gsLoggedIn;
            logger->info("Logged In");
            
            if(statuslistener != NULL)
                statuslistener->loggedIn(true);
        }else if(frame->getType() == ft03_Redirect){
            status = gsDisconnected;
            sock->disconnect();
            if(statuslistener == NULL || (statuslistener != NULL && statuslistener->redirected(static_cast<Redirect*>(frame)->getUrl()))){
                connect(static_cast<Redirect*>(frame)->getUrl());
            }
            delete frame;
        }else{
            logger->error("Login failed: %s", ((FailFrame*)frame)->getErrorString().c_str());
            //TODO tp04 fail frame refs?
            if(statuslistener != NULL){
                statuslistener->loggedIn(false);
            }
        }
    }
    
    void GameLayer::accountCreateCallback(Frame* frame){
        if(frame->getType() == ft02_OK){
            delete frame;
            logger->info("Account created");
            
            if(statuslistener != NULL){
                statuslistener->accountCreated(true);
            }
        }else{
            delete frame;
            logger->error("Account Creation failed: %s", ((FailFrame*)frame)->getErrorString().c_str());
            //TODO tp04 fail frame refs?
            if(statuslistener != NULL){
                statuslistener->accountCreated(false);
            }
        }
    }
    
    void GameLayer::timeRemainingCallback(Frame* frame){
        if(frame->getType() == ft02_Time_Remaining){
            if(statuslistener != NULL){
                statuslistener->timeToEot(((TimeRemaining*)frame)->getTimeRemaining());
            }
        }
        delete frame;
    }
    
    void GameLayer::finishedTurnCallback(Frame* frame){
        //check the frame type
        //disable sending finishedturn frames if fail frame.
        delete frame;
    }
    
    void GameLayer::gameInfoCallback(Frame* frame){
        
    }
    
}
