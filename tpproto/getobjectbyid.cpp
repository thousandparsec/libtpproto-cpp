

#include "buffer.h"

#include "getobjectbyid.h"

namespace TPProto{

  GetObjectByID::GetObjectByID(){
    idset.clear();
  }

  GetObjectByID::~GetObjectByID(){
    idset.clear();
  }

  void GetObjectByID::packBuffer(Buffer* buf){
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
    type = ft02_Object_GetById;
  }

  bool GetObjectByID::unpackBuffer(Buffer* buf){
    return false;
  }

  void GetObjectByID::addObjectID(unsigned int id){
    idset.insert(id);
  }

  void GetObjectByID::addObjectIDs(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

}
