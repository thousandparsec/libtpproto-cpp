#ifndef TPPROTO_TLSSOCKET_H
#define TPPROTO_TLSSOCKET_H

/*! \file
  \brief Declares TlsSocket class.
*/

#include <string>
#include <tpproto/tpsocket.h>

typedef struct gnutls_session_int* gnutls_session_t;
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
    
    void send(char* header, int hlen, char *data, int len);
    int recvHeader(int len, char* &data);
    int recvBody(int len, char* &data);

    bool poll();

    void setServerAddr(const char* host, const char* port);

    protected:
    virtual bool onConnect();
    bool connect(const std::string& host, const std::string& port);
    int sockfd;
    int status;
    char* hostname;
    char* portname;
    gnutls_session_t session;
    gnutls_certificate_credentials_t credentials;
  };

}

#endif
