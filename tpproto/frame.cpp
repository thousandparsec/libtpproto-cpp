

#include "buffer.h"

#include "frame.h"

namespace TPProto{

  Frame::~Frame(){}

  void Frame::setProtocolVersion(int ver){
    protoVer = ver;
  }

  int Frame::getProtocolVersion(){
    return protoVer;
  }

  int Frame::getType(){
    return type;
  }

}
