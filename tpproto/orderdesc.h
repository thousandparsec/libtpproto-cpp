#ifndef TPPROTO_ORDERDESC_H
#define TPPROTO_ORDERDESC_H

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

  class OrderDescription : public Frame{
  public:
    OrderDescription();
    virtual ~OrderDescription();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);
    
    unsigned int getOrderType();
    std::string getName();
    std::string getDescription();
    //get args

  private:
    int otype;
    std::string name;
    std::string desc;

  };

}

#endif
