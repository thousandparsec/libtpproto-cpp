

#include "objectvisitor.h"

#include "galaxy.h"

namespace TPProto{

  Galaxy::Galaxy(){
  }

  Galaxy::~Galaxy(){
  }

  void Galaxy::visit(ObjectVisitor* vis){
    vis->visit(this);
  }

}
