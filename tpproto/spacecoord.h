#ifndef TPPROTO_SPACECOORD_H
#define TPPROTO_SPACECOORD_H

#include <tpproto/orderparameter.h>

namespace TPProto{

  class SpaceCoordinates : public OrderParameter{
  public:
    SpaceCoordinates();
    SpaceCoordinates(const SpaceCoordinates& rhs);
    virtual ~SpaceCoordinates();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    long long getX();
    long long getY();
    long long getZ();

    void setAll(long long nx, long long ny, long long nz);

  private:
    long long x;
    long long y;
    long long z;

  };

}

#endif
