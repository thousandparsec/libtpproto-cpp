#ifndef TPPROTO_SEQUENCE_H
#define TPPROTO_SEQUENCE_H

/*! \file
  \brief Declares the Sequence Frame.
*/

#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief Sent by the server to indicate the number of Frames in a set to
    follow.
  */
  class Sequence : public Frame{
  public:
    Sequence();
    virtual ~Sequence();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    int getNumber();

  private:
    int num;

  };

}

#endif
