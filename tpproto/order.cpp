

#include "buffer.h"

#include "order.h"

namespace TPProto{

  Order::Order(){
  }

  Order::~Order(){
  }

  void Order::packBuffer(Buffer* buf){
    buf->packInt(object);
    buf->packInt(slot);
    buf->packInt(otype);
    buf->packInt(0); // numturns
    buf->packInt(0); // resource list

    //pack paramters

    type = ft02_Order_Insert;
  }

  bool Order::unpackBuffer(Buffer* buf){
    object = buf->unpackInt();
    slot = buf->unpackInt();
    otype = buf->unpackInt();
    numturns = buf->unpackInt();
    //resource lists are currently not used...
    buf->unpackInt(); // that had better be 0...
    
    //unpack parameters

    type = ft02_Order;
    return true;
  }

  unsigned int Order::getObjectId(){
    return object;
  }

  int Order::getSlot(){
    return slot;
  }

  unsigned int Order::getType(){
    return otype;
  }

  unsigned int Order::getNumTurns(){
    return numturns;
  }

  int Order::getNumParameters(){
    return 0;
  }


}

