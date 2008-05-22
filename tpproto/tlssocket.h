#ifndef TPPROTO_TLSSOCKET_H
#define TPPROTO_TLSSOCKET_H

/*! \file
  \brief Declares TlsSocket class.
*/

#include <string>
#include <tpproto/tpsocket.h>

/*! \brief The gnutls session structure.
*/
typedef struct gnutls_session_int* gnutls_session_t;
/*! \brief The gnutls certificate credentials structure.
*/
typedef struct gnutls_certificate_credentials_st *gnutls_certificate_credentials_t;

namespace TPProto{

  /*! \brief A TPSocket for connecting over TLS over TCP to a server.
   */
  class TlsSocket : public TPSocket{
  public:
    TlsSocket();
    virtual ~TlsSocket();

    bool isConnected();
    
    void disconnect();
    
    int send(const char *data, int len);
    int recv(int len, char* &data);

    void setServerAddr(const char* host, const char* port);

    protected:
    virtual bool onConnect();
    bool connect(const std::string& host, const std::string& port);

    /*! \brief The status of the connection.
        Set to 0 when disconnected.
    */
    int status;
    /*! \brief The hostname (or address) to connect to.
    */
    char* hostname;
    /*! \brief The portname (or number) to connect to.
    */
    char* portname;
    /*! \brief The TLS session structure for this connection.
    */
    gnutls_session_t session;
    /*! \brief The TLS credentials structure for this connection.
    */
    gnutls_certificate_credentials_t credentials;
  };

}

#endif
