#ifndef TPPROTO_SPACECOORD_H
#define TPPROTO_SPACECOORD_H

#include <tpproto/vector3d.h>
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

    Vector3d getPos();

    void setPos(const Vector3d& npos);

  private:
    Vector3d pos;

  };

}

#endif
