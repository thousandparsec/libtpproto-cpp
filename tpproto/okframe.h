#ifndef TPPROTO_OKFRAME_H
#define TPPROTO_OKFRAME_H

/*! \file
  \brief Declares the OkFrame Frame.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{
  
  /*! \brief The OkFrame is sent from the server to say that an operation was
    successful.
  */
    class OkFrame : public Frame{
  public:
        OkFrame();
        virtual ~OkFrame();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);
    
    std::string getInfo();

    protected:
    /*! \brief The information string sent by the server.
    */
    std::string info;

  };

}

#endif
