

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

  void Frame::setSequenceNumber(unsigned int seq){
    seqnum = seq;
  }

  unsigned int Frame::getSequenceNumber(){
    return seqnum;
  }

  int Frame::getType(){
    return type;
  }

}
