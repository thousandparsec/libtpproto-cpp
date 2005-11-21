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

  /*! \brief Fetches the OrderDescription for the given OrderTypes.
    
  /param otypes Set of order types to get for the cache.
  */
// void FrameCodec::seedOrderDescriptionCache(std::set<unsigned int> otypes){
//     
//     for(std::set<unsigned int>::iterator itcurr = otypes.begin(); 
//         itcurr != otypes.end(); ++itcurr){
//             if(orderdescCache[*itcurr] != NULL){
//                 std::set<unsigned int>::iterator itnew = itcurr;
// 	// this is not ideal...
//                 if(itcurr == otypes.begin())
//                     ++itnew;
//                 else
//                     --itnew;
//                 
//                 otypes.erase(itcurr);
//                 itcurr = itnew;
//                 
//             }
//         }
//     
//     if(!otypes.empty()){
//         GetOrderDescription* god = protocol->getFrameFactory()->createGetOrderDescription();
//         god->addOrderTypes(otypes);
//         uint32_t seqnum = sendFrame(god);
//         delete god;
//         
//         std::list<Frame*> reply = recvFrames(seqnum);
//         for(std::list<Frame*>::iterator itcurr = reply.begin(); itcurr != reply.end(); ++itcurr){
//             Frame * ob = *itcurr;
//             
//             if(ob != NULL && ob->getType() == ft02_OrderDesc){
//                 OrderDescription* od = (OrderDescription*)ob;
//                 orderdescCache[od->getOrderType()] = od;
//             }else{
//                 logger->debug("Expected Order Desc frame, got %d", ob->getType());
//             }
//         }
//         
//     }
// }

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
