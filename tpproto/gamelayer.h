#ifndef TPPROTO_GAMELAYER_H
#define TPPROTO_GAMELAYER_H
/*  GameLayer - High level interface to the Thousand Parsec game.
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

#include <string>
#include <set>
#include <map>
#include <list>
/*! \file
  \brief Declares the GameLayer class.

  It is the main file you will need to include to access high level functions.
*/

namespace TPProto{

    class Logger;
    class ProtocolLayer;
    class TPSocket;
    class Order;
    class Message;
    class Features;
    class GameStatusListener;
    class GameLayerAsyncFrameListener;
    class CacheMethod;
    class ObjectDescCache;
    class ObjectCache;
    class OrderDescCache;
    class PlayerCache;
    class BoardCache;
    class ResourceCache;
    class CategoryCache;
    class DesignCache;
    class ComponentCache;
    class PropertyCache;
    class EventLoop;

    /*! \brief GameStatus enum for the state of the game.
    */
    enum GameStatus{
        gsDisconnected,
        gsConnecting,
        gsConnected,
        gsLoggedIn,
        gsEOTInProgress
    };

    /*! \brief GameLayer is the main interface to the higher layer of libtpproto-cpp.

        GameLayer abstracts away most of the underlying details of how Objects and Orders
        are requested and received.
    */
    class GameLayer{
    public:
        friend class GameLayerAsyncFrameListener;

        GameLayer();
        ~GameLayer();

        //config & setup

        void setClientString(const std::string & name);
        void setLogger(Logger* nlog);
        void setGameStatusListener(GameStatusListener* gsl);
        void setCacheMethod(CacheMethod* prototype);
        void setEventLoop(EventLoop* el);

        //status
        GameStatus getStatus();
        
        //Protocol Layer
        ProtocolLayer* getProtocolLayer() const;

        //connect, login and disconnect
        bool connect(const std::string& address);
        bool connect(TPSocket* nsock);
        bool createAccount(const std::string &user, const std::string &password, const std::string &email, const std::string &comment);
        bool login(const std::string &user, const std::string &password);
        void disconnect();

        void updateCaches();
    

        //ObjectDescriptions
        ObjectDescCache* getObjectDescCache() const;
        
        //Objects
       ObjectCache* getObjectCache() const;
       
       //OrderDescription
       OrderDescCache* getOrderDescCache() const;

        //Orders
        std::list<Order*> getOrders(uint32_t obid, uint32_t num);
        Order* getOrder(uint32_t obid, uint32_t slot);
        Order* createOrderFrame(int type);
        bool insertOrder(Order* frame);
        bool replaceOrder(Order* frame);
        Order* probeOrder(Order* frame);
        bool removeOrder(uint32_t obid, uint32_t slot);

        //Boards and Messages
        BoardCache* getBoardCache() const;
        std::list<Message*> getMessages(uint32_t boardid, uint32_t num);
        Message* createMessage();
        Message* getMessage(uint32_t boardid, uint32_t slot);
        bool postMessage(Message* frame);
        bool removeMessage(uint32_t boardid, uint32_t slot);

        //Resource types
        ResourceCache* getResourceCache() const;
        
        //Players
        PlayerCache* getPlayerCache() const;
        
        //Designs
        CategoryCache* getCategoryCache() const;
        DesignCache* getDesignCache() const;
        ComponentCache* getComponentCache() const;
        PropertyCache* getPropertyCache() const;
        
        //Time
        void getTimeRemaining();
        void finishedTurn();

    private:
        
        void connectCallback(Frame* frame);
        void featureCallback(Frame* frame);
        void loginCallback(Frame* frame);
        void accountCreateCallback(Frame* frame);
        void timeRemainingCallback(Frame* frame);
        void finishedTurnCallback(Frame* frame);
        
        ProtocolLayer* protocol;
        EventLoop* eventloop;
        Logger* logger;
        GameStatusListener* statuslistener;
        TPSocket* sock;
        GameStatus status;
        std::string clientid;

        Features* serverfeatures;

        GameLayerAsyncFrameListener* asyncframes;

        ObjectDescCache* objectdesccache;
        ObjectCache* objectcache;
        OrderDescCache* orderdesccache;
        PlayerCache* playercache;
        BoardCache* boardcache;
        ResourceCache* resourcecache;
        CategoryCache* categorycache;
        DesignCache* designcache;
        ComponentCache* componentcache;
        PropertyCache* propertycache;
    };

}

#endif
