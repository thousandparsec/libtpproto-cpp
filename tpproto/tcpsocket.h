#ifndef TPPROTO_TCPSOCKET_H
#define TPPROTO_TCPSOCKET_H

#include <tpproto/tpsocket.h>

namespace TPProto{

  class TcpSocket : public TPSocket{
  public:
    TcpSocket();
    virtual ~TcpSocket();

    bool isConnected();
    
    bool connect();
    void disconnect();
    
    void send(char* header, int hlen, char *data, int len);
    int recvHeader(int len, char* &data);
    int recvBody(int len, char* &data);

    void setServerAddr(const char* host, const char* port = "6923");

  private:
    int sockfd;
    int status;
    char* hostname;
    char* portname;
    
  };

}

#endif
