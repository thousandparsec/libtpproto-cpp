

#include "buffer.h"
#include "orderparametervisitor.h"

#include "spacecoord.h"

namespace TPProto{

  SpaceCoordinates::SpaceCoordinates(){
  }

  SpaceCoordinates::SpaceCoordinates(const SpaceCoordinates& rhs) : OrderParameter(rhs){
    pos = rhs.pos;
  }

  SpaceCoordinates::~SpaceCoordinates(){
  }

  void SpaceCoordinates::packBuffer(Buffer* buf){
    pos.pack(buf);
  }

  bool SpaceCoordinates::unpackBuffer(Buffer* buf){
    pos.unpack(buf);
    return true;
  }

  OrderParameter* SpaceCoordinates::clone(){
    return new SpaceCoordinates(*this);
  }

  void SpaceCoordinates::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  Vector3d SpaceCoordinates::getPos(){
    return pos;
  }

  void SpaceCoordinates::setPos(const Vector3d& npos){
    pos = npos;
  }

}

