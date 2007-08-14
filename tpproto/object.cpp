
#include <cassert>

#include "buffer.h"
#include "objectvisitor.h"

#include "object.h"

namespace TPProto{

  /*! \brief Required virtual destructor.
   */
  Object::~Object(){
  }

  /*! \brief Asserts(0).
    
  Thie method asserts zero because it should never be sent to the server.
  /param buf The Buffer (ignored).
  */
  void Object::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpacks the standard Object fields from the Buffer.

  Everything except the ObjectType specific data is unpacked.
  /param buf The Buffer the Object is to be unpacked from.
  \return True if Successful.
  */
  bool Object::unpackBuffer(Buffer* buf){
    id = buf->unpackInt();
    /* FIXME: Can we check this exists?? */
    obtype = buf->unpackInt();
    char* strbuf = buf->unpackString();
    name = strbuf;
    delete[] strbuf;
    size = buf->unpackInt64();
    pos.unpack(buf);
    vel.unpack(buf);
    /* FIXME: Sanity check? */
    int count = buf->unpackInt();
    contained.clear();
    for(int i = 0; i < count; i++){
      contained.insert(buf->unpackInt());
    }
    availableorders.clear();
    /* FIXME: Sanity check? */
    count = buf->unpackInt();
    for(int i = 0; i < count; i++){
      availableorders.insert(buf->unpackInt());
    }
    numorders = buf->unpackInt();
    //4 unint32 padding (TP02), or modtime and 2 uint32 padding (TP03)
    if(protoVer == 2){
        buf->unpackInt();
        buf->unpackInt();
    }else{
        modtime = buf->unpackInt64();
    }
    buf->unpackInt();
    buf->unpackInt();

    type = ft02_Object;

    return true;

  }

  /*! \brief Gets the Object's id number.
    \return The id number.
  */
  unsigned int Object::getId(){
    return id;
  }

  /*! \brief Gets the Object's name.
    \return The name of the object.
  */
  std::string Object::getName(){
    return name;
  }

  /*! \brief Gets the position of the object.
    \return Vector3d of the position.
  */
  Vector3d Object::getPos(){
    return pos;
  }

  /*! \brief Gets the velocity of the object.
    \return Vector3d of the velocity.
  */
  Vector3d Object::getVel(){
    return vel;
  }

  /*! \brief Gets the object type number of the object.
    \return The object type number.
  */
  unsigned int Object::getObjectType(){
    return obtype;
  }

  /*! \brief Gets the diameter of the object in units.
    \return The diameter of the object.
  */
  uint64_t Object::getSize(){
    return size;
  }

  /*! \brief Gets the set of contained objectids.
    \return The set of contained object ids.
  */
  std::set<unsigned int> Object::getContainedObjectIds(){
    return contained;
  }

  /*! \brief Gets the set of available Order types.
    \return The set of order types.
  */
  std::set<unsigned int> Object::getAvailableOrders(){
    return availableorders;
  }

  /*! \brief Gets the number of Orders on this object.

  The ids of the Orders are from 0 to numorders-1.
  \return The number of orders.
  */
  unsigned int Object::getNumberOrders(){
    return numorders;
  }

  /*! \brief Gets the last time this object was modified.
  \return The timestamp the object was last modified.
  */
  uint64_t Object::getLastModifiedTime(){
    return modtime;
  }

}

