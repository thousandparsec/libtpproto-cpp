#ifndef TPPROTO_TIMEREMAINING_H
#define TPPROTO_TIMEREMAINING_H

/*! \file
  \brief Declares the TimeRemaining Frame.
*/

#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief Notification of the time remaining before the end of turn.
   */
  class TimeRemaining : public Frame{
  public:
    TimeRemaining();
    virtual ~TimeRemaining();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    int getTimeRemaining();

  private:
    int time;

  };

}

#endif
