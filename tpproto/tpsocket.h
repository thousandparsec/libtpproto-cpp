#ifndef TPPROTO_TPSOCKET_H
#define TPPROTO_TPSOCKET_H

namespace TPProto{

  class TPSocket{
  public:
    virtual ~TPSocket();
    
    virtual bool isConnected();

    virtual bool connect() = 0;
    virtual void disconnect() = 0;

    virtual void send(char* header, int hlen, char* data, int len) = 0;
    virtual int recvHeader(int len, char* &data) = 0;
    virtual int recvBody(int len, char * &data) = 0;

    virtual bool poll();

  };

}

#endif
