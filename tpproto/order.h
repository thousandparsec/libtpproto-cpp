#ifndef TPPROTO_ORDER_H
#define TPPROTO_ORDER_H

#include <tpproto/frame.h>

namespace TPProto{

  class Order : public Frame{
  public:
    Order();
    virtual ~Order();
    
    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    unsigned int getObjectId();
    int getSlot();
    unsigned int getType();
    unsigned int getNumTurns();

    int getNumParameters();
    //OrderParameter* getParameter(int which);

  private:
    unsigned int object;
    int slot;
    unsigned int otype;
    unsigned int numturns;

  };

}

#endif
