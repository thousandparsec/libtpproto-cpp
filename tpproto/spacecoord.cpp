

#include "buffer.h"
#include "orderparametervisitor.h"

#include "spacecoord.h"

namespace TPProto{

  SpaceCoordinates::SpaceCoordinates(){
    x = 0ll;
    y = 0ll;
    z = 0ll;
  }

  SpaceCoordinates::SpaceCoordinates(const SpaceCoordinates& rhs) : OrderParameter(rhs){
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
  }

  SpaceCoordinates::~SpaceCoordinates(){
  }

  void SpaceCoordinates::packBuffer(Buffer* buf){
    buf->packInt64(x);
    buf->packInt64(y);
    buf->packInt64(z);
  }

  bool SpaceCoordinates::unpackBuffer(Buffer* buf){
    x = buf->unpackInt64();
    y = buf->unpackInt64();
    z = buf->unpackInt64();

    return true;
  }

  OrderParameter* SpaceCoordinates::clone(){
    return new SpaceCoordinates(*this);
  }

  void SpaceCoordinates::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  long long SpaceCoordinates::getX(){
    return x;
  }

  long long SpaceCoordinates::getY(){
    return y;
  }
  
  long long SpaceCoordinates::getZ(){
    return z;
  }

  void SpaceCoordinates::setAll(long long nx, long long ny, long long nz){
    x = nx;
    y = ny;
    z = nz;
  }

}

