#ifndef TESTSOCKET_H
#define TESTSOCKET_H

#include <tpproto/tpsocket.h>

class TestSocket : public TPProto::TPSocket{
 public:
  TestSocket();
  virtual ~TestSocket();
  
  bool isConnected();

  bool connect();
  void disconnect();

  void send(char* header, int hlen, char* data, int len);
  int recvHeader(int len, char* &data);
  int recvBody(int len, char* &data);

 private:
  bool connected;
  int stage;
  int seqnum;

};

#endif
