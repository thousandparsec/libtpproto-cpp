
#include "buffer.h"

#include "getorderdesc.h"

namespace TPProto{

  GetOrderDescription::GetOrderDescription(){
  }

  GetOrderDescription::~GetOrderDescription(){
    idset.clear();
  }

  void GetOrderDescription::packBuffer(Buffer* buf){
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }

    type = ft02_OrderDesc_Get;
  }

  bool GetOrderDescription::unpackBuffer(Buffer* buf){
    return false;
  }

  void GetOrderDescription::addOrderType(unsigned int id){
    idset.insert(id);
  }

  void GetOrderDescription::addOrderTypes(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

}

