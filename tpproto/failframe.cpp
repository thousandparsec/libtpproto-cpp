
#include <cassert>

#include "buffer.h"

#include "failframe.h"

namespace TPProto{

  FailFrame::FailFrame(){
    errcode = 0;
    errstring = "";
  }

  FailFrame::~FailFrame(){

  }

  void FailFrame::packBuffer(Buffer *buf){
    // should never send an Fail frame
    assert(0);
  }

  bool FailFrame::unpackBuffer(Buffer *buf){
    errcode = buf->unpackInt();
    char* temp = buf->unpackString();
    errstring = temp;
    delete[] temp;
    
    type = ft02_Fail;

    return true;
  }

  int FailFrame::getErrorCode(){
    return errcode;
  }

  std::string FailFrame::getErrorString(){
    return errstring;
  }

}
