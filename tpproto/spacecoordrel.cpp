

#include "buffer.h"
#include "orderparametervisitor.h"

#include "spacecoordrel.h"

namespace TPProto{

  SpaceCoordinatesRel::SpaceCoordinatesRel(){
  }

  SpaceCoordinatesRel::SpaceCoordinatesRel(const SpaceCoordinatesRel& rhs) : OrderParameter(rhs){
    pos = rhs.pos;
  }

  SpaceCoordinatesRel::~SpaceCoordinatesRel(){
  }

  void SpaceCoordinatesRel::packBuffer(Buffer* buf){
    buf->packInt(objectid);
    pos.pack(buf);
  }

  bool SpaceCoordinatesRel::unpackBuffer(Buffer* buf){
    objectid = buf->unpackInt();
    pos.unpack(buf);
    return true;
  }

  OrderParameter* SpaceCoordinatesRel::clone(){
    return new SpaceCoordinatesRel(*this);
  }

  void SpaceCoordinatesRel::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  Vector3d SpaceCoordinatesRel::getPos(){
    return pos;
  }

  unsigned int SpaceCoordinatesRel::getObjectId(){
    return objectid;
  }

  void SpaceCoordinatesRel::setPos(const Vector3d& npos){
    pos = npos;
  }

  void SpaceCoordinatesRel::setObjectId(unsigned int oid){
    objectid = oid;
  }

}

