#ifndef TPPROTO_CONNECTION_H
#define TPPROTO_CONNECTION_H
/*  Connection - Base class for objects handling sockets
 *
 *  Copyright (C) 2008  Lee Begg and the Thousand Parsec Project
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

/*! \file
  \brief Declares the Connection class.
 */

namespace TPProto{

    class TPSocket;
    class EventLoop;
    
    /*! \brief Abstract class for reading and writing to sockets
    
    The protocol releated to the this connection and socket is implemented in a subclass
    of Connection.
    */
    class Connection{
      public:
        Connection();
        virtual ~Connection();
        
        void setSocket(TPSocket* sock);
        void setEventLoop(EventLoop* el);
        
        /*! \brief Called when the socket has data ready to be read
        
        Protocol reading entry point is here. Non-blocking sockets should be used.
        */
        virtual void readyToRead() = 0;
        
        /*! \brief Called when the socket is ready for data to be sent to it
        
        This can be used to send queued data when non-blocking writes say
        they would have blocked.
        */
        virtual void readyToSend() = 0;
        
      protected:
        /*! \brief The TPSocket this connection is connected to.
        */
        TPSocket* socket;
        /*! \brief The EventLoop to use.
        */
        EventLoop* eventloop;
    };
}

#endif
