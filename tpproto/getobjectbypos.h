#ifndef TPPROTO_GETOBJECTBYPOS_H
#define TPPROTO_GETOBJECTBYPOS_H

/*! \file
  \brief Declares GetObjectByPos Frame class.
*/

#include <stdint.h>

#include <tpproto/frame.h>
#include <tpproto/vector3d.h>

namespace TPProto{

  /*! \brief Frame for getting all objects within a given area.
    
  The area is a sphere of radius units centre at the position.
  */
  class GetObjectByPos : public Frame{
  public:
    GetObjectByPos();
    virtual ~GetObjectByPos();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setPosition(const Vector3d &npos);
    void setRadius(uint64_t nrad);

  private:
    Vector3d pos;
    uint64_t rad;

  };

}

#endif
