
#include <cassert>

#include "buffer.h"
#include "orderparameter.h"

#include "orderdesc.h"

namespace TPProto{

  OrderDescription::OrderDescription(){
  }

  OrderDescription::~OrderDescription(){
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      delete *itcurr;
    }
    params.clear();
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

  std::list<OrderParameter*> OrderDescription::getParameters(){
    std::list<OrderParameter*> np;
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      np.push_back((*itcurr)->clone());
    }
    return np;
  }

}

