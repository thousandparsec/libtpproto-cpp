#ifndef TPPROTO_ORDERDESC_H
#define TPPROTO_ORDERDESC_H

#include <string>
#include <list>
#include <tpproto/frame.h>

namespace TPProto{

  class OrderParameter;

  class OrderDescription : public Frame{
  public:
    OrderDescription();
    virtual ~OrderDescription();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);
    
    unsigned int getOrderType();
    std::string getName();
    std::string getDescription();
    std::list<OrderParameter*> getParameters();

  private:
    int otype;
    std::string name;
    std::string desc;
    std::list<OrderParameter*> params;

  };

}

#endif
