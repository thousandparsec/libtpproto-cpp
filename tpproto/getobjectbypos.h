#ifndef TPPROTO_GETOBJECTBYPOS_H
#define TPPROTO_GETOBJECTBYPOS_H

#include <tpproto/getobjects.h>
#include <tpproto/vector3d.h>

namespace TPProto{

  class GetObjectByPos : public GetObjects{
  public:
    GetObjectByPos();
    virtual ~GetObjectByPos();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setPosition(const Vector3d &npos);
    void setRadius(unsigned long long nrad);

  private:
    Vector3d pos;
    unsigned long long rad;

  };

}

#endif
