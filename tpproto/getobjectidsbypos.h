#ifndef TPPROTO_GETOBJECTIDSBYPOS_H
#define TPPROTO_GETOBJECTIDSBYPOS_H

/*! \file
  \brief Declares GetObjectIdsByPos Frame class.
*/

#include <stdint.h>

#include <tpproto/frame.h>
#include <tpproto/vector3d.h>

namespace TPProto{

    /*! \brief Frame for getting all object ids within a given area.

    The area is a sphere of radius units centre at the position.
    */
    class GetObjectIdsByPos : public Frame{
    public:
        GetObjectIdsByPos();
        virtual ~GetObjectIdsByPos();

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
