
#include <cassert>

#include "buffer.h"

#include "okframe.h"

namespace TPProto{

  OKFrame::OKFrame(){
    info = "";
  }

  OKFrame::~OKFrame(){

  }

  void OKFrame::packBuffer(Buffer *buf){
    // should never send an OK frame
    assert(0);
  }

  bool OKFrame::unpackBuffer(Buffer *buf){
    char* temp = buf->unpackString();
    info = temp;
    delete[] temp;
    
    type = ft02_OK;

    return true;
  }

  std::string OKFrame::getInfo(){
    return info;
  }

}
