

#include "buffer.h"

#include "connect.h"

namespace TPProto{

  Connect::Connect(){
    clientstring = "";
  }

  Connect::~Connect(){
  }

  void Connect::packBuffer(Buffer* buf){
    buf->packString(clientstring.c_str());
    type = ft02_Connect;
  }

  bool Connect::unpackBuffer(Buffer* buf){
    // should never receive this frame
    return false;
  }

  void Connect::setClientString(const std::string &cs){
    clientstring = cs;
  }

}
