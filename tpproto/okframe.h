#ifndef TPPROTO_OKFRAME_H
#define TPPROTO_OKFRAME_H

#include <string>
#include <tpproto/frame.h>

namespace TPProto{
  
  class OKFrame : public Frame{
  public:
    OKFrame();
    virtual ~OKFrame();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);
    
    std::string getInfo();

  private:
    std::string info;

  };

}

#endif
