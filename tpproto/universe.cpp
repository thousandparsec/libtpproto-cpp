

#include "buffer.h"
#include "objectvisitor.h"

#include "universe.h"

namespace TPProto{

  Universe::Universe(){

  }

  Universe::~Universe(){

  }

  bool Universe::unpackBuffer(Buffer *buf){
    Object::unpackBuffer(buf);
    age = buf->unpackInt();
    return true;
  }

  void Universe::visit(ObjectVisitor * vis){
    vis->visit(this);
  }

  int Universe::getAge(){
    return age;
  }

}
