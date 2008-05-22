#ifndef TPPROTO_TCPSOCKET_H
#define TPPROTO_TCPSOCKET_H

/*! \file
  \brief Declares TcpSocket class.
*/

#include <tpproto/tpsocket.h>

namespace TPProto{

  /*! \brief A TPSocket for connecting over TCP to a server.
   */
  class TcpSocket : public TPSocket{
  public:
    TcpSocket();
    virtual ~TcpSocket();

    bool isConnected();
    
    bool connect();
    void disconnect();
    
    int send(const char *data, int len);
    int recv(int len, char* &data);

    void setServerAddr(const char* host, const char* port = "6923");

  private:
    int status;
    char* hostname;
    char* portname;
    
  };

}

#endif
