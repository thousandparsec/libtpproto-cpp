

#include "buffer.h"
#include "orderparameter.h"
#include "orderdesc.h"

#include "order.h"

namespace TPProto{

  Order::Order(){
  }

  Order::~Order(){
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      delete *itcurr;
    }
    params.clear();
  }

  void Order::packBuffer(Buffer* buf){
    buf->packInt(object);
    buf->packInt(slot);
    buf->packInt(desc->getOrderType());
    buf->packInt(0); // numturns
    buf->packInt(0); // resource list

    //pack paramters
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      (*itcurr)->packBuffer(buf);
    }

    type = ft02_Order_Insert;
  }

  bool Order::unpackBuffer(Buffer* buf){
    object = buf->unpackInt();
    slot = buf->unpackInt();
    if(buf->unpackInt() != desc->getOrderType())
      return false;
    numturns = buf->unpackInt();
    //resource lists are currently not used...
    buf->unpackInt(); // that had better be 0...
    
    //unpack parameters
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      (*itcurr)->unpackBuffer(buf);
    }

    type = ft02_Order;
    return true;
  }

  std::string Order::getName(){
    return desc->getName();
  }

  std::string Order::getDescription(){
    return desc->getDescription();
  }

  unsigned int Order::getOrderType(){
    return desc->getOrderType();
  }

  unsigned int Order::getObjectId(){
    return object;
  }

  int Order::getSlot(){
    return slot;
  }

  unsigned int Order::getNumTurns(){
    return numturns;
  }

  unsigned int Order::getNumParameters(){
    return params.size();
  }

  OrderParameter* Order::getParameter(unsigned int which){
    if(which < params.size()){
      std::list<OrderParameter*>::iterator itcurr = params.begin();
      advance(itcurr, which);
      if(itcurr != params.end()){
	return *itcurr;
      }
    }
    
    return NULL;
    
  }

  void Order::setObjectId(unsigned int oid){
    object = oid;
  }

  void Order::setSlot(int s){
    slot = s;
  }

  void Order::setOrderType(OrderDescription* od){
    desc = od;
    params = od->getParameters();
  }

}

