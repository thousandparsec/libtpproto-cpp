#ifndef TPPROTO_GAMELAYER_H
#define TPPROTO_GAMELAYER_H
/*  GameLayer - High level interface to the Thousand Parsec game.
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
    class Object;
    class Order;
    class Board;
    class Message;
    class ResourceDescription;
    class Player;
    class Category;
    class Design;
    class Component;
    class Property;
    class Features;
    class GameStatusListener;
    class GameLayerAsyncFrameListener;
    class ObjectCache;
    class PlayerCache;
    class BoardCache;

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

        //status
        GameStatus getStatus();

        //connect, login and disconnect
        bool connect(const std::string& address);
        bool connect(TPSocket* nsock);
        bool login(const std::string &user, const std::string &password);
        void disconnect();
    

        //Objects
        std::set<uint32_t> getObjectIds();
        Object* getObject(uint32_t obid);
        Object* getUniverse();

        //Orders
        std::list<Order*> getOrders(uint32_t obid, uint32_t num);
        Order* getOrder(uint32_t obid, uint32_t slot);
        Order* createOrderFrame(int type);
        bool insertOrder(Order* frame);
        bool replaceOrder(Order* frame);
        Order* probeOrder(Order* frame);
        bool removeOrder(uint32_t obid, uint32_t slot);

        //Boards and Messages
        std::set<uint32_t> getBoardIds();
        Board* getBoard(uint32_t boardid);
        Board* getPersonalBoard();
        std::list<Message*> getMessages(uint32_t boardid, uint32_t num);
        Message* createMessage();
        Message* getMessage(uint32_t boardid, uint32_t slot);
        bool postMessage(Message* frame);
        bool removeMessage(uint32_t boardid, uint32_t slot);

        //Resource types
        ResourceDescription* getResourceDescription(uint32_t restype);
        
        //Players
        Player* getPlayer(uint32_t playerid);
        
        //Designs
        std::set<uint32_t> getCategoryIds();
        Category* getCategory(uint32_t catid);
        Category* createCategory();
        bool addCategory(Category* cat);
        bool removeCategory(uint32_t catid);
        std::set<uint32_t> getDesignIds();
        Design* getDesign(uint32_t designid);
        Design* createDesign();
        bool addDesign(Design* d);
        bool modifyDesign(Design* d);
        bool removeDesign(uint32_t designid);
        std::set<uint32_t> getComponentIds();
        Component* getComponent(uint32_t compid);
        std::set<uint32_t> getPropertyIds();
        Property* getProperty(uint32_t propid);
        
        //Time
        int getTimeRemaining();

    private:
        ProtocolLayer* protocol;
        Logger* logger;
        GameStatusListener* statuslistener;
        TPSocket* sock;
        GameStatus status;
        std::string clientid;

        Features* serverfeatures;

        GameLayerAsyncFrameListener* asyncframes;

        ObjectCache* objectcache;
        PlayerCache* playercache;
        BoardCache* boardcache;
    };

}

#endif
