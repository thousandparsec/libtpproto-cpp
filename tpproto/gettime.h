#ifndef TPPROTO_GETTIME_H
#define TPPROTO_GETTIME_H

/*! \file
  \brief Declares GetTime Frame.
*/

#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief A Frame for getting the time remaining before the end of turn.
   */
  class GetTime : public Frame{
  public:
    GetTime();
    virtual ~GetTime();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    
  };

}

#endif
