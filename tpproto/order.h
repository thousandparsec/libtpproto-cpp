#ifndef TPPROTO_ORDER_H
#define TPPROTO_ORDER_H

#include <list>
#include <tpproto/frame.h>

namespace TPProto{

  class OrderParameter;

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

    unsigned int getNumParameters();
    OrderParameter* getParameter(unsigned int which);

    void setObjectId(unsigned int oid);
    void setSlot(int s);
    void setOrderType(int t);
    void setParameters(const std::list<OrderParameter*>& nparams);

  private:
    unsigned int object;
    int slot;
    unsigned int otype;
    unsigned int numturns;

    std::list<OrderParameter*> params;

  };

}

#endif
