#ifndef TPPROTO_HTTPSSOCKET_H
#define TPPROTO_HTTPSSOCKET_H

/*! \file
  \brief Declares HttpsSocket class.
*/

#include <tpproto/tpsocket.h>

typedef struct gnutls_session_int* gnutls_session_t;
typedef struct gnutls_certificate_credentials_st *gnutls_certificate_credentials_t;

namespace TPProto{

  /*! \brief A TPSocket for connecting over TLS over TCP to a server.
   */
  class HttpsSocket : public TPSocket{
  public:
    HttpsSocket();
    virtual ~HttpsSocket();

    bool isConnected();
    
    bool connect();
    void disconnect();
    
    void send(char* header, int hlen, char *data, int len);
    int recvHeader(int len, char* &data);
    int recvBody(int len, char* &data);

    bool poll();

    void setServerAddr(const char* host, const char* port = "443");
    static void setProxyAddr(const char* host, const char* port = "3128");

  private:
    int sockfd;
    int status;
    char* hostname;
    char* portname;
    static char* proxyhost;
    static char* proxyport;
    gnutls_session_t session;
    gnutls_certificate_credentials_t credentials;
  };

}

#endif
