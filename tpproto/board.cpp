
#include <cassert>

#include "buffer.h"

#include "board.h"

namespace TPProto{

  Board::Board(){
  }

  Board::~Board(){
  }

  void Board::packBuffer(Buffer *buf){
    assert(0);
  }

  bool Board::unpackBuffer(Buffer* buf){
    id = buf->unpackInt();
    char* temp = buf->unpackString();
    name = temp;
    delete[] temp;
    temp = buf->unpackString();
    desc = temp;
    delete[] temp;

    nummess = buf->unpackInt();

    type = ft02_Board;

    return true;
  }

  unsigned int Board::getId(){
    return id;
  }

  std::string Board::getName(){
    return name;
  }

  std::string Board::getDescription(){
    return desc;
  }

  int Board::numMessages(){
    return nummess;
  }
  
}
