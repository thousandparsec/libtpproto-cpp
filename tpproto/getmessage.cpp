

#include "buffer.h"

#include "getmessage.h"

namespace TPProto{

  GetMessage::GetMessage(){
  }

  GetMessage::~GetMessage(){
    idset.clear();
  }

  void GetMessage::packBuffer(Buffer* buf){
    buf->packInt(bid);
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Message_Get;
  }

  bool GetMessage::unpackBuffer(Buffer* buf){
    return false;
  }

  void GetMessage::setBoard(unsigned int board){
    bid = board;
  }

  void GetMessage::addMessageId(unsigned int id){
    idset.insert(id);
  }

  void GetMessage::addMessageIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

  void GetMessage::addMessageRange(unsigned int start, unsigned int end){
    for(unsigned int i = start; i < end; i++){
      idset.insert(i);
    }
  }

}

