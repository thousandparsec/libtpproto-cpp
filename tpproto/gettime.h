#ifndef TPPROTO_GETTIME_H
#define TPPROTO_GETTIME_H

#include <tpproto/frame.h>

namespace TPProto{

  class GetTime : public Frame{
  public:
    GetTime();
    virtual ~GetTime();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    
  };

}

#endif
