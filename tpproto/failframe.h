#ifndef TPPROTO_FAILFRAME_H
#define TPPROTO_FAILFRAME_H

#include <string>
#include <tpproto/frame.h>

namespace TPProto{
  
  class FailFrame : public Frame{
  public:
    FailFrame();
    virtual ~FailFrame();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);
    
    int getErrorCode();
    std::string getErrorString();

  private:
    int errcode;
    std::string errstring;

  };

}

#endif
