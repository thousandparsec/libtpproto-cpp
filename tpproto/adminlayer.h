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
#include <set>
#include <map>
#include <list>
/*! \file
  \brief Declares the AdminLayer class.
*/

namespace TPProto{

    class ProtocolLayer;
    class TPSocket;
    class Command;
    class AdminLayerAsyncFrameListener;
    class CacheMethod;
    class CommandDescCache;
    class EventLoop;

    enum AdminStatus
    {
        asDisconnected,
        asConnecting,
        asConnected,
        asLoggedIn
    };

    class AdminLayer{
    public:
        friend class AdminLayerAsyncFrameListener;

        AdminLayer();
        ~AdminLayer();

        // config and setup
        void setClientString(const std::string & name);
        void setLogger(Logger * nlog);
        void setCacheMethod(CacheMethod* prototype);
        void setEventLoop(EventLoop* el);

        // protocol layer
        ProtocolLayer* getProtocolLayer() const;

        // connect, login and disconnect
        bool connect(const std::string& address);
        bool connect(TPSocket* nsock);
        bool login(const std::string &user, const std::string &password);
        void disconnect();

        void updateCaches();
    
        //CommandDescription
        CommandDescCache* getCommandDescCache() const;

    private:
        void connectCallback(Frame* frame);
        void loginCallback(Frame* frame);
        
        ProtocolLayer * protocol;
        EventLoop * eventloop;
        Logger * logger;
        TPSocket * sock;
        AdminStatus status;
        std::string clientid;

        AdminLayerAsyncFrameListener* asyncframes;

        CommandDescCache* commanddesccache;
    };

}

#endif
