#ifndef TPPROTO_TPSSOCKET_H
#define TPPROTO_TPSSOCKET_H

/*! \file
  \brief Declares TpsSocket class.
*/

#include <tpproto/tlssocket.h>

namespace TPProto{

  /*! \brief A TPSocket for connecting over TLS over TCP to a TPS server.
   */
  class TpsSocket : public TlsSocket{
  public:
    TpsSocket();
    virtual ~TpsSocket();
    
    bool connect();

    void setServerAddr(const char* host, const char* port = "6924");

  };

}

#endif
