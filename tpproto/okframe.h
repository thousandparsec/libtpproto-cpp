#ifndef TPPROTO_OKFRAME_H
#define TPPROTO_OKFRAME_H

/*! \file
  \brief Declares the OKFrame Frame.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{
  
  /*! \brief The OKFrame is sent from the server to say that an operation was
    successful.
  */
  class OKFrame : public Frame{
  public:
    OKFrame();
    virtual ~OKFrame();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);
    
    std::string getInfo();

  private:
    std::string info;

  };

}

#endif
