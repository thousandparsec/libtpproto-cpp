#ifndef TPPROTO_SEQUENCE_H
#define TPPROTO_SEQUENCE_H

#include <tpproto/frame.h>

namespace TPProto{

  class Sequence : public Frame{
  public:
    Sequence();
    virtual ~Sequence();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    int getNumber();

  private:
    int num;

  };

}

#endif
