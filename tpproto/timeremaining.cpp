
#include <cassert>

#include "buffer.h"

#include "timeremaining.h"

namespace TPProto{

  TimeRemaining::TimeRemaining(){
  }
  
  TimeRemaining::~TimeRemaining(){
  }

  void TimeRemaining::packBuffer(Buffer* buf){
    assert(0);
  }

  bool TimeRemaining::unpackBuffer(Buffer* buf){
    time = buf->unpackInt();
    type = ft02_Time_Remaining;
    return true;
  }

  int TimeRemaining::getTimeRemaining(){
    return time;
  }

}

