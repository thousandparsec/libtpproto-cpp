
#include <cassert>

#include "buffer.h"
#include "objectvisitor.h"

#include "object.h"

namespace TPProto{

  Object::~Object(){
  }

  void Object::packBuffer(Buffer* buf){
    assert(0);
  }

  bool Object::unpackBuffer(Buffer* buf){
    id = buf->unpackInt();
    obtype = buf->unpackInt();
    char* strbuf = buf->unpackString();
    name = strbuf;
    delete strbuf;
    size = buf->unpackInt64();
    pos.unpack(buf);
    vel.unpack(buf);
    int count = buf->unpackInt();
    contained.clear();
    for(int i = 0; i < count; i++){
      contained.insert(buf->unpackInt());
    }
    availableorders.clear();
    count = buf->unpackInt();
    for(int i = 0; i < count; i++){
      availableorders.insert(buf->unpackInt());
    }
    numorders = buf->unpackInt();
    //4 unint32 padding
    buf->unpackInt();
    buf->unpackInt();
    buf->unpackInt();
    buf->unpackInt();

    type = ft02_Object;

    return true;

  }

  unsigned int Object::getId(){
    return id;
  }

  std::string Object::getName(){
    return name;
  }

  Vector3d Object::getPos(){
    return pos;
  }

  Vector3d Object::getVel(){
    return vel;
  }

  int Object::getObjectType(){
    return obtype;
  }

  unsigned long long Object::getSize(){
    return size;
  }

  std::set<unsigned int> Object::getContainedObjectIds(){
    return contained;
  }

  std::set<unsigned int> Object::getAvailableOrders(){
    return availableorders;
  }

  int Object::getNumberOrders(){
    return numorders;
  }

}

