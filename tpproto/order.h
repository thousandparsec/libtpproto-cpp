#ifndef TPPROTO_ORDER_H
#define TPPROTO_ORDER_H

/*! \file
  \brief Declares Order Frame.
*/

#include <list>
#include <string>
#include <tpproto/frame.h>

namespace TPProto{

  class OrderParameter;
  class OrderDescription;

  /*! \brief A Frame representing an Order on an Object.
   */
  class Order : public Frame{
  public:
    Order();
    virtual ~Order();
    
    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    std::string getName();
    std::string getDescription();
    unsigned int getOrderType();

    unsigned int getObjectId();
    int getSlot();
    unsigned int getNumTurns();

    unsigned int getNumParameters();
    OrderParameter* getParameter(unsigned int which);

    void setObjectId(unsigned int oid);
    void setSlot(int s);
    void setOrderType(OrderDescription* od);

  private:
    unsigned int object;
    int slot;
    unsigned int otype;
    unsigned int numturns;

    std::list<OrderParameter*> params;

    OrderDescription* desc;

  };

}

#endif
