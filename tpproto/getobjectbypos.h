#ifndef TPPROTO_GETOBJECTBYPOS_H
#define TPPROTO_GETOBJECTBYPOS_H

/*! \file
  \brief Declares GetObjectByPos Frame class.
*/

#include <tpproto/getobjects.h>
#include <tpproto/vector3d.h>

namespace TPProto{

  /*! \brief Frame for getting all objects within a given area.
    
  The area is a sphere of radius units centre at the position.
  */
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
