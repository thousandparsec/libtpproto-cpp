
#include "objectvisitor.h"

#include "starsystem.h"

namespace TPProto{

  StarSystem::StarSystem(){
  }

  StarSystem::~StarSystem(){
  }

  void StarSystem::visit(ObjectVisitor* vis){
    vis->visit(this);
  }

}
