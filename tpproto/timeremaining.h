#ifndef TPPROTO_TIMEREMAINING_H
#define TPPROTO_TIMEREMAINING_H

#include <tpproto/frame.h>

namespace TPProto{

  class TimeRemaining : public Frame{
  public:
    TimeRemaining();
    virtual ~TimeRemaining();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    int getTimeRemaining();

  private:
    int time;

  };

}

#endif
