
#include "buffer.h"
#include "objectvisitor.h"

#include "fleet.h"

namespace TPProto{

  Fleet::Fleet(){
  }

  Fleet::~Fleet(){
    ships.clear();
  }

  bool Fleet::unpackBuffer(Buffer* buf){
    Object::unpackBuffer(buf);
    owner = buf->unpackInt();
    
    int num = buf->unpackInt();
    for(int i = 0; i < num; i++){
      int stype = buf->unpackInt();
      ships[stype] = buf->unpackInt();
    }

    damage = buf->unpackInt();

    return true;
  }

  void Fleet::visit(ObjectVisitor* vis){
    vis->visit(this);
  }

  int Fleet::getOwner(){
    return owner;
  }

  int Fleet::getDamage(){
    return damage;
  }

  int Fleet::numShips(int stype){
    return ships[stype];
  }


}
