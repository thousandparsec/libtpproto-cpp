#ifndef TPPROTO_GETOBJECTS_H
#define TPPROTO_GETOBJECTS_H

/*! \file
  \brief Declares the GetObjects baseclass
*/

#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief The GetObjects base class.

  This class is used to make to treat GetObjectByID and GetObjectByPos
  the same way in FrameCodec.  It doesn't actually do anything.
  */
  class GetObjects : public Frame{
  public:
    /*! \brief Required virtual destructor.
     */
    virtual ~GetObjects(){};
  };

}

#endif
