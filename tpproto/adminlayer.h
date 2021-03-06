#ifndef TPPROTO_ADMINLAYER_H
#define TPPROTO_ADMINLAYER_H
/*  AdminLayer - High level interface for server administration.
 *
 *  Copyright (C) 2008 Aaron Mavrinac and the Thousand Parsec Project
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
#include <list>
#include <boost/shared_ptr.hpp>
/*! \file
  \brief Declares the AdminLayer class.
*/

namespace TPProto{

    class Logger;
    class ProtocolLayer;
    class TPSocket;
    class Frame;
    class Command;
    class CommandParameter;
    class CommandDescription;
    class AdminStatusListener;
    class AdminLayerAsyncFrameListener;
    class CacheMethod;
    class CommandDescCache;
    class EventLoop;

    /*! \brief Enum for the Status of the Adminlayer.
    */
    enum AdminStatus
    {
        asDisconnected,
        asConnecting,
        asConnected,
        asLoggedIn
    };

    /*! \brief They layer for interact with the Administration interface of the server.
    Basically the same as the GameLayer, but for the Administration protocol.
    */
    class AdminLayer{
    public:
        friend class AdminLayerAsyncFrameListener;

        AdminLayer();
        ~AdminLayer();

        // config and setup
        void setClientString(const std::string & name);
        void setLogger(Logger * nlog);
        void setAdminStatusListener(AdminStatusListener* asl);
        void setCacheMethod(CacheMethod * prototype);
        void setEventLoop(EventLoop * el);

        // status
        AdminStatus getStatus();

        // protocol layer
        ProtocolLayer* getProtocolLayer() const;

        // connect, login and disconnect
        bool connect(const std::string& address);
        bool connect(TPSocket * nsock);
        bool login(const std::string &user, const std::string &password);
        void disconnect();

        // send command
        void sendCommand(boost::shared_ptr<CommandDescription> cd, std::list<CommandParameter*> plist);

        void updateCaches();
    
        //CommandDescription
        CommandDescCache* getCommandDescCache() const;

    private:
        void connectCallback(Frame * frame);
        void loginCallback(Frame * frame);
        
        void commandCallback(Frame * frame);
        
        ProtocolLayer * protocol;
        EventLoop * eventloop;
        Logger * logger;
        AdminStatusListener * statuslistener;
        TPSocket * sock;
        AdminStatus status;
        std::string clientid;

        AdminLayerAsyncFrameListener* asyncframes;

        CommandDescCache* commanddesccache;
    };

}

#endif
