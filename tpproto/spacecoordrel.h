#ifndef TPPROTO_SPACECOORDREL_H
#define TPPROTO_SPACECOORDREL_H

#include <tpproto/vector3d.h>
#include <tpproto/orderparameter.h>

namespace TPProto{

  class SpaceCoordinatesRel : public OrderParameter{
  public:
    SpaceCoordinatesRel();
    SpaceCoordinatesRel(const SpaceCoordinatesRel& rhs);
    virtual ~SpaceCoordinatesRel();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    Vector3d getPos();
    unsigned int getObjectId();

    void setPos(const Vector3d& npos);
    void setObjectId(unsigned int oid);

  private:
    unsigned int objectid;
    Vector3d pos;

  };

}

#endif
