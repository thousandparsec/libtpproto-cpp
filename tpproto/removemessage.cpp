

#include "buffer.h"

#include "removemessage.h"

namespace TPProto{

  RemoveMessage::RemoveMessage(){
  }

  RemoveMessage::~RemoveMessage(){
    idset.clear();
  }

  void RemoveMessage::packBuffer(Buffer* buf){
    buf->packInt(bid);
    buf->packInt(idset.size());
    for(std::set<unsigned int>::reverse_iterator itcurr = idset.rend(); itcurr != idset.rbegin(); --itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Message_Remove;
  }

  bool RemoveMessage::unpackBuffer(Buffer* buf){
    return false;
  }

  void RemoveMessage::setBoard(unsigned int board){
    bid = board;
  }

  void RemoveMessage::removeMessageId(unsigned int id){
    idset.insert(id);
  }

  void RemoveMessage::removeMessageIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

  void RemoveMessage::removeMessageRange(unsigned int start, unsigned int end){
    for(unsigned int i = start; i < end; i++){
      idset.insert(i);
    }
  }

}
