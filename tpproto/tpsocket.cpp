

#include "tpsocket.h"

namespace TPProto{
  
  TPSocket::~TPSocket(){}
  
  bool TPSocket::isConnected(){
    return false;
  }

  bool TPSocket::poll(){
    return false;
  }
  
}
