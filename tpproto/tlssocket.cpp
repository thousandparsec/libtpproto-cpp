
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <errno.h>
#include <gnutls/gnutls.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tlssocket.h"

namespace TPProto{

  /*! \brief Default constructor.

  Sets up defaults, that will prevent connecting.
  */
  TlsSocket::TlsSocket(){
    status = 0;
    hostname = NULL;
    portname = NULL;
    gnutls_global_init ();
    gnutls_certificate_allocate_credentials (&credentials);
    gnutls_certificate_set_x509_trust_file (credentials, "ca-cert.pem", GNUTLS_X509_FMT_PEM);
  }

  /*! \brief Destructor.
   */
  TlsSocket::~TlsSocket(){
    if(hostname != NULL)
      delete[] hostname;
    if(portname != NULL)
      delete[] portname;
    gnutls_certificate_free_credentials (credentials);
    gnutls_global_deinit ();
  }

  /*! \brief Check if the socket is connected.
    \return True if connected, false otherwise.
  */
  bool TlsSocket::isConnected(){
    return status == 1;
  }

  /*! \brief Connects to the host and port over TCP.
    
  <p>If HAVE_IPV6 is defined, it will use either IPv4 or IPv6 to
  make the connection, otherwise it will only use IPv4 (old APIs). They follow 
  different code paths.</p>
  <p>After a connection is made, but before TLS is negotiated the virtual
  method onConnect is called.</p>
  \param host The host to connect to.
  \param port The port to connect on.
  \return True if a new connection is made, false if already connected or
  if an error occured.
  */
    bool TlsSocket::connect(const std::string& host, const std::string& port){
        if(status == 0 && !host.empty() && !port.empty()){
      
            gnutls_init (&session, GNUTLS_CLIENT);
        
        
            gnutls_set_default_priority (session);
            const int cert_type_priority[3] = { GNUTLS_CRT_X509,
                //GNUTLS_CRT_OPENPGP, 
                0
            };
            gnutls_certificate_type_set_priority (session, cert_type_priority);
            /* put the x509 credentials to the current session
            */
            gnutls_credentials_set (session, GNUTLS_CRD_CERTIFICATE, credentials);
        
            gnutls_set_default_priority (session);

    #ifdef HAVE_IPV6
      
            struct addrinfo hints, *res, *ressave;
            int n;
        
            memset(&hints, 0, sizeof(struct addrinfo));
        
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            
            n = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
            
            if (n <0) {
                fprintf(stderr,
                        "getaddrinfo error:: [%s]\n",
                        gai_strerror(n));
                return false;
            }
            
            ressave = res;
            
            fd=-1;
            while (res) {
                fd = socket(res->ai_family,
                                res->ai_socktype,
                                res->ai_protocol);
                
                if (!(fd < 0)) {
                    if (::connect(fd, res->ai_addr, res->ai_addrlen) == 0)
                        break;
                
                    close(fd);
                    fd=-1;
                }
                res=res->ai_next;
            }
            
            freeaddrinfo(ressave);
        
#else
// IPv4 only
        
            struct sockaddr_in sin;
            struct hostent *phe;
            struct servent *pse;
            
            memset(&sin, 0, sizeof(sin));
            
            sin.sin_family=AF_INET;
            
            if ( pse = getservbyname(port.c_str(), "tcp") ) {
                sin.sin_port = pse->s_port;
                
            } else if ((sin.sin_port = htons((u_short)atoi(port.c_str())))==0) {
                fprintf(stderr, "ipv4_only_connect:: could not get service=[%s]\n",
                        post.c_str());
                return false;
            }
            
            if (phe = gethostbyname(host.c_str())) {
                memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
                
            } else if ( (sin.sin_addr.s_addr = inet_addr(host.c_str())) == 
                        INADDR_NONE) {
                fprintf(stderr, "ipv4_only_connect:: could not get host=[%s]\n", host.c_str());
                return false;
            }
            
            if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
                fprintf(stderr, "ipv4_only_connect:: could not open socket\n");
                return false;
            }
            
            if (connect(fd,(struct sockaddr *)&sin, sizeof(sin)) < 0) {
                fprintf(stderr, "ipv4_only_connect:: could not connect to host=[%s]\n", host.c_str());
                return false;
            }
        
    #endif
        
            if(!(fd < 0) && onConnect()){
                gnutls_transport_set_ptr (session, (gnutls_transport_ptr_t) fd);
                int ret = gnutls_handshake (session);
    
                if (ret < 0){
                    fprintf (stderr, "*** Handshake failed\n");
                    gnutls_perror (ret);
                    close(fd);
                    fd = 0;
                    gnutls_deinit (session);
                    return false;
                }
                
                //check cert. TODO
                
                status = 1;
                return true;
            }
        }
        
        
        return false;
  }

  /*! \brief Closes the connection.

  The connection is closed after calling this method (even if it was already
  closed).
  */
  void TlsSocket::disconnect(){
    if(status == 1){
        gnutls_bye (session, GNUTLS_SHUT_RDWR);
        close(fd);
        gnutls_deinit (session);
        fd = 0;
    }
    status = 0;
  }

  /*! \brief Sends data to the socket.

  \throw DisconnectionException if not connected.
  \param data The data.
  \param len The length of the data.
  \return The number of bytes sent.
  */
    int TlsSocket::send(const char* data, int len){
        if(status != 1){
            throw new DisconnectedException();
        }
        int slen = gnutls_record_send(session, data, len);
        if(slen == 0){
            status = 0;
            disconnect();
            throw new DisconnectedException();
        }
        if(slen < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            return 0;
        return slen;
  }

  /*! \brief Receives data from the network.

  \throw DisconnectionException if not connected.
    \param len The length of the header to receive.
    \param data The array to store the data in.
    \return The length written into the data array.
    */
    int TlsSocket::recv(int len, char* &data){
        if(status != 1){
            throw new DisconnectedException();
        }
        data = (char*)malloc(len);
        int rlen = gnutls_record_recv(session, data, len);
        if(rlen == 0){
            status = 0;
            disconnect();
            throw new DisconnectedException();
        }
        if(rlen < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            return 0;
        return rlen;
    }

  /*! \brief Sets the server address and port to connect to.

  Host can be DNS name or ip address. Post can be port name or number.
  \param host Host to connect to.
  \param port Port to connect to.
  */
  void TlsSocket::setServerAddr(const char* host, const char* port){
    if(hostname != NULL){
      delete hostname;
    }
    if(portname != NULL){
      delete portname;
    }
    hostname = new char[strlen(host) + 1];
    strncpy(hostname, host, strlen(host) + 1);
    portname = new char[strlen(port) + 1];
    strncpy(portname, port, strlen(port) + 1);
  }

  /*! \brief Called by connect when connection is established by before TLS handshake.
    \return True to continue, false if there is an error and need to abort.
  */
    bool TlsSocket::onConnect(){
        return true;
    }
  
}
