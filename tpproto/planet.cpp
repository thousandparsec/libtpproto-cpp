
#include "buffer.h"
#include "objectvisitor.h"

#include "planet.h"

namespace TPProto{

  Planet::Planet(){
  }

  Planet::~Planet(){
  }

  bool Planet::unpackBuffer(Buffer* buf){
    Object::unpackBuffer(buf);
    owner = buf->unpackInt();
    return true;
  }

  void Planet::visit(ObjectVisitor* vis){
    vis->visit(this);
  }

  int Planet::getOwner(){
    return owner;
  }

}
