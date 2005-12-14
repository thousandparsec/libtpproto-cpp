#ifndef TPPROTO_HTTPSSOCKET_H
#define TPPROTO_HTTPSSOCKET_H

/*! \file
  \brief Declares HttpsSocket class.
*/

#include <tpproto/tlssocket.h>

namespace TPProto{

  /*! \brief A TPSocket for connecting over TLS over TCP to a server.
   */
  class HttpsSocket : public TlsSocket{
  public:
    HttpsSocket();
    virtual ~HttpsSocket();
    
    bool connect();

    void setServerAddr(const char* host, const char* port = "443");
    static void setProxyAddr(const char* host, const char* port = "3128");

protected:
    bool onConnect();

  private:
    static char* proxyhost;
    static char* proxyport;
  };

}

#endif
