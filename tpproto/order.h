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
    unsigned int getOrderType();
    unsigned int getNumTurns();

    int getNumParameters();
    //OrderParameter* getParameter(int which);

    void setObjectId(unsigned int oid);
    void setSlot(int s);
    void setOrderType(int t);

  private:
    unsigned int object;
    int slot;
    unsigned int otype;
    unsigned int numturns;

  };

}

#endif
