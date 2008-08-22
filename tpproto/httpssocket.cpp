
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifndef VERSION
#define VERSION "0.0.0"
#endif

#include "httpssocket.h"

namespace TPProto{

    char* HttpsSocket::proxyhost = NULL;
    char* HttpsSocket::proxyport = NULL;

  /*! \brief Default constructor.

  Sets up defaults, that will prevent connecting.
  */
  HttpsSocket::HttpsSocket() : TlsSocket(){
  }

  /*! \brief Destructor.
   */
  HttpsSocket::~HttpsSocket(){
  }

  /*! \brief Connects to the host and port over HTTPS tunnel.
  \return True if a new connection is made, false if already connected or
  if an error occured.
  */
  bool HttpsSocket::connect(){
        if(proxyhost != NULL && proxyport != NULL){
            return TlsSocket::connect(proxyhost, proxyport);
        }else{
            return TlsSocket::connect(hostname, portname);
        }
    }
        
    bool HttpsSocket::onConnect(){
        //init connection over HTTP proxy
        if(proxyhost != NULL && proxyport != NULL){
            std::string connectstring("CONNECT ");
            connectstring += std::string(hostname) + ":" + portname + " HTTP/1.0\r\nUser-agent: libtpproto-cpp/" VERSION "\r\n\r\n";
            send(connectstring.c_str(), connectstring.length());
            char buff[1024];
            char* ptrbuff = buff;
            uint len = recv(1024, ptrbuff);
            buff[len] = '\0';
            //TODO check buffer, make sure we got "HTTP/1.0 200"...
        }
        return true;
    }

  /*! \brief Sets the server address and port to connect to.

  Host can be DNS name or ip address. Post can be port name or number.
  \param host Host to connect to.
  \param port Port to connect to.
  */
  void HttpsSocket::setServerAddr(const char* host, const char* port){
        TlsSocket::setServerAddr(host, port);
  }

    /*! \brief Sets the proxy address and port to connect to.

    Host can be DNS name or ip address. Post can be port name or number.
    \param host Host to connect to.
    \param port Port to connect to.
    */
    void HttpsSocket::setProxyAddr(const char* host, const char* port){
        if(proxyhost != NULL){
        delete proxyhost;
        }
        if(proxyport != NULL){
        delete proxyport;
        }
        proxyhost = new char[strlen(host) + 1];
        strncpy(proxyhost, host, strlen(host) + 1);
        proxyport = new char[strlen(port) + 1];
        strncpy(proxyport, port, strlen(port) + 1);
    }

}
