
#include "buffer.h"

#include "getboard.h"

namespace TPProto{

  GetBoard::GetBoard(){
  }

  GetBoard::~GetBoard(){
    idset.clear();
  }

  void GetBoard::packBuffer(Buffer* buf){
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Board_Get;
  }

  bool GetBoard::unpackBuffer(Buffer* buf){
    return false;
  }

  void GetBoard::addBoardId(unsigned int id){
    idset.insert(id);
  }

  void GetBoard::addBoardIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

}

