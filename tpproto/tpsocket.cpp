/*  TPSocket - Base class sockets in libtpproto-cpp
 *
 *  Copyright (C) 2004, 2005, 2008  Lee Begg and the Thousand Parsec Project
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

#include <cstdlib>

#include "connection.h"

#include "tpsocket.h"

namespace TPProto{
  
    /*! Constructor
    */
    TPSocket::TPSocket() : fd(0), connection(NULL){
    }
    
    /*! \brief Required virtual destructor.
    */
    TPSocket::~TPSocket(){
        if(connection != NULL){
            connection->setSocket(NULL);
        }
    }

    /*! \brief Gets the FileDescriptor.
    
    Used by toolkits to add them to the event loop.
    \return The file descriptor.
    */
    int TPSocket::getFileDescriptor() const{
        return fd;
    }
    
    /*! \brief Sets the Connection
    
    Set the connection object associated with this connection.
    \param conn The Connection object.
    */
    void TPSocket::setConnection(Connection* conn){
        connection = conn;
        connection->setSocket(this);
    }
    
    /*! Gets the Connection associated with this Socket.
    \return The Connection object.
    */
    Connection* TPSocket::getConnection() const{
        return connection;
    }

    /*! \brief Call when the socket is ready to send data
    
    This method should be called when the socket is ready to send data.
    @throws DisconnectedException if the socket is disconnected.
    
    */
    void TPSocket::readyToSend(){
        if(connection != NULL)
            connection->readyToSend();
        if(!isConnected()){
            throw new DisconnectedException();
        }
    }
    
    /*! \brief Call when the socket has data ready to be read.
    
    This method should be called when the socket is read to have data read.
    @throws DisconnectedException if the socket id disconnected.
    */
    void TPSocket::readyToRead(){
        if(connection != NULL)
            connection->readyToRead();
        if(!isConnected()){
            throw new DisconnectedException();
        }
    }
  
}
