
#include <iostream>
#include <netinet/in.h>

#include "testsocket.h"

using namespace TPProto;

TestSocket::TestSocket(){
  connected = false;
  stage = 0;
}

TestSocket::~TestSocket(){

}

bool TestSocket::isConnected(){
  return connected;
}

bool TestSocket::connect(){
  if(connected == true)
    return false;
  else{
    connected = true;
    return true;
  }
}

void TestSocket::disconnect(){
  connected = false;
}

void TestSocket::send(char* header, int hlen, char* data, int len){
  std::cout << "FileSocket::send(): Header length: " << hlen << " Data Length: " << len << std::endl;
  int dlen = ntohl(*(unsigned int*)(header + 12));
  if(dlen != len){
    std::cout << "FileSocket::send(): Data length mismatch, header said " << dlen << " param said " << len << std::endl;
  }
  int type = ntohl(*(unsigned int*)(header + 8));
  seqnum = ntohl(*(unsigned int*)(header+4));
  switch(stage){
  case 0:
    if(type == 3){
      stage = 1;
    }else{
      std::cout << "FileSocket::send(): Waiting for Connect but got frame of type " << type << std::endl;
    }
    break;
  case 1:
    if(type == 4){
      stage = 2;
    }else{
      std::cout << "FileSocket::send(): Waiting for login but got frame of type " << type << std::endl;
    }
    break;
  default:
    std::cout << "FileSocket::send(): Unknown Stage" << std::endl;
    break;
  }
}

int TestSocket::recvHeader(int len, char* &data){
  int type, blen;
  
  switch(stage){
  case 0:
    type = 1;
    blen = 17;
    break;

  case 1:
  case 2:
    type = 0;
    blen = 7;
    break;

  default:

    break;
  }

  data = new char[16];
  data[0] = 'T';
  data[1] = 'P';
  data[2] = '0';
  data[3] = '2';
  int temp = htonl(seqnum);
  memcpy(data + 4, &temp, 4);
  temp = htonl(type);
  memcpy(data + 8, &temp, 4);
  temp = htonl(blen);
  memcpy(data + 12, &temp, 4);

  return 16;
}

int TestSocket::recvBody(int len, char* &data){
  
  data = new char[len];

  switch(stage){
  case 0:
    memcpy(data, "\0\0\0\020\0\0\0\001\0\0\006Error\0", 17);
    break;

  case 1:
  case 2:
    memcpy(data, "\0\0\0\003OK\0", 7);
    break;

  default:
    break;
  }

  return len;
}
