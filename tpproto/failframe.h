#ifndef TPPROTO_FAILFRAME_H
#define TPPROTO_FAILFRAME_H

/*! \file
  \brief Declares FailFrame class.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{
  
  /*! \brief Represenation of fail or error Frame from the server.
   */
  class FailFrame : public Frame{
  public:
    FailFrame();
    virtual ~FailFrame();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);
    
    int getErrorCode();
    std::string getErrorString();

  private:
    int errcode;
    std::string errstring;

  };

}

#endif
