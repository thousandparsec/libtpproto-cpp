
#include <cassert>

#include "buffer.h"

#include "sequence.h"

namespace TPProto{

  Sequence::Sequence(){

  }

  Sequence::~Sequence(){

  }

  void Sequence::packBuffer(Buffer* buf){
    assert(0);
  }

  bool Sequence::unpackBuffer(Buffer* buf){
    num = buf->unpackInt();
    return true;
  }

  int Sequence::getNumber(){
    return num;
  }

}

