#ifndef TPPROTO_TPSOCKET_H
#define TPPROTO_TPSOCKET_H

/*! \file
  \brief Declares the TPSocket base class
*/

namespace TPProto{

  /*! \brief The TPSocket base class.

  All sockets that the client can communicate are a TPSocket.  This allows
  any current and future sockets (such as the ssl, http and https sockets 
  in TP03) to be handled in the same way.  This abstraction also allows 
  for testing using a file socket or a program or code acting like a socket.
  */
  class TPSocket{
  public:
    virtual ~TPSocket();
    
    virtual bool isConnected();

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
    \param header The header data to send.
    \param hlen The length of the header in bytes.
    \param data The data to send.
    \param len The length of the data to send.
    */
    virtual void send(char* header, int hlen, char* data, int len) = 0;

    /*! \brief Receive a header from the socket.

    All subclasses must override this method.  For some sockets it will be
    identical to recvBody().
    \param len The length of the header to receive.
    \param data The array to store the data in.
    \return The length written into the data array.
    */
    virtual int recvHeader(int len, char* &data) = 0;
    
    /*! \brief Receive the data from the socket.

    All subclasses must override this method.  For some sockets it will be
    identical to recvHeader().
    \param len The length of the data to receive.
    \param data The array to store the data in.
    \return The length written into the data array.
    */
    virtual int recvBody(int len, char * &data) = 0;

    virtual bool poll();

  };

}

#endif
