#ifndef TPPROTO_CONNECT_H
#define TPPROTO_CONNECT_H

/*! \file
  \brief Declares Connect class.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{
  
  /*! \brief Connect Frame class.

    This class is the Connect Frame sent when creating a connection to the
    server.  It sends a string to the server in the data.  FrameCodec is
    the preferred way of setting the client string as it also adds the library
    version.
  */
  class Connect : public Frame{
  public:
    Connect();
    virtual ~Connect();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setClientString(const std::string &cs);

    private:
    std::string clientstring;
    

  };

}

#endif
