

#include "buffer.h"

#include "gettime.h"

namespace TPProto{

  GetTime::GetTime(){
  }

  GetTime::~GetTime(){
  }

  void GetTime::packBuffer(Buffer* buf){
    type = ft02_Time_Remaining_Get;
  }

  bool GetTime::unpackBuffer(Buffer* buf){
    return false;
  }

}

