

#include "buffer.h"

#include "removeorder.h"

namespace TPProto{

  RemoveOrder::RemoveOrder(){
  }

  RemoveOrder::~RemoveOrder(){
    idset.clear();
  }

  void RemoveOrder::packBuffer(Buffer* buf){
    buf->packInt(object);
    buf->packInt(idset.size());
    for(std::set<unsigned int>::reverse_iterator itcurr = idset.rend(); itcurr != idset.rbegin(); --itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Order_Remove;
  }

  bool RemoveOrder::unpackBuffer(Buffer* buf){
    return false;
  }

  void RemoveOrder::setObjectId(unsigned int obid){
    object = obid;
  }

  void RemoveOrder::removeOrderId(unsigned int id){
    idset.insert(id);
  }

  void RemoveOrder::removeOrderIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

  void RemoveOrder::removeOrderRange(unsigned int start, unsigned int end){
    for(unsigned int i = start; i < end; i++){
      idset.insert(i);
    }
  }

}
