
#include <cassert>

#include "buffer.h"

#include "orderdesc.h"

namespace TPProto{

  OrderDescription::OrderDescription(){
  }

  OrderDescription::~OrderDescription(){
  }

  void OrderDescription::packBuffer(Buffer* buf){
    assert(0);
  }

  bool OrderDescription::unpackBuffer(Buffer* buf){
    otype = buf->unpackInt();
    char* temp = buf->unpackString();
    name = temp;
    delete temp;
    temp = buf->unpackString();
    desc = temp;
    delete temp;

    //unpack args
    
    type = ft02_OrderDesc;

    return true;
  }

  unsigned int OrderDescription::getOrderType(){
    return otype;
  }

  std::string OrderDescription::getName(){
    return name;
  }

  std::string OrderDescription::getDescription(){
    return desc;
  }

  

}

