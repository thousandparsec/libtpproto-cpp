

#include "buffer.h"

#include "getorder.h"

namespace TPProto{

  GetOrder::GetOrder(){
  }

  GetOrder::~GetOrder(){
    idset.clear();
  }

  void GetOrder::packBuffer(Buffer* buf){
    buf->packInt(object);
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Order_Get;
  }

  bool GetOrder::unpackBuffer(Buffer* buf){
    return false;
  }

  void GetOrder::setObjectId(unsigned int obid){
    object = obid;
  }

  void GetOrder::addOrderId(unsigned int id){
    idset.insert(id);
  }

  void GetOrder::addOrderIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

  void GetOrder::addOrderRange(unsigned int start, unsigned int end){
    for(unsigned int i = start; i < end; i++){
      idset.insert(i);
    }
  }

}

