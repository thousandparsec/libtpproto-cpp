#ifndef TPPROTO_TPSOCKET_H
#define TPPROTO_TPSOCKET_H
/*  TPSocket - sockets abstraction for libtpproto-cpp
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

#include <exception>

/*! \file
  \brief Declares the TPSocket base class
*/

namespace TPProto{
    
    class Connection;
    
    class DisconnectedException : public std::exception{
    };

  /*! \brief The TPSocket base class.

  All sockets that the client can communicate are a TPSocket.  This allows
  any current and future sockets (such as the ssl, http and https sockets 
  in TP03) to be handled in the same way.  This abstraction also allows 
  for testing using a file socket or a program or code acting like a socket.
  */
  class TPSocket{
  public:
      TPSocket();
    virtual ~TPSocket();
    
    /*! \brief Checks if the socket is connected.

    All subclasses must override this method and track whether they 
    are connected or not.
    \return True if connected, false otherwise.
     */
    virtual bool isConnected() = 0;

    int getFileDescriptor() const;
    
    void setConnection(Connection* conn);
    Connection* getConnection() const;
    
    /*! \brief Connects to the server.

    All subclasses must override this method.
    \return True if the socket connected successfully, false otherwise.
    */
    virtual bool connect() = 0;

    /*! \brief Disconnects from the server.

    All subclasses must override this method.
    */
    virtual void disconnect() = 0;


    /*! \brief sends some data to the server.

    All subclasses must override this method.
    \param data The data to send.
    \param len The length of the data to send.
    \return The number of bytes sent.
    */
    virtual int send(const char* data, int len) = 0;

    /*! \brief Receive the data from the socket.

    All subclasses must override this method.
    \param len The length of the data to receive.
    \param data The array to store the data in.
    \return The length written into the data array.
    */
    virtual int recv(int len, char * data) = 0;

    void readyToSend();
    void readyToRead();

      protected:
          /*! The file descriptor
          */
          int fd;
          
          /*! The connection object for this socket.
          */
          Connection * connection;

  };

}

#endif
