
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
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
    sockfd = -1;
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
    
  If HAVE_IPV6 is defined, it will use either IPv4 or IPv6 to
  make the connection, otherwise it will only use IPv4 (old APIs). They follow 
  different code paths.
  \return True if a new connection is made, false if already connected or
  if an error occured.
  */
  bool TlsSocket::connect(){
    if(status == 0 && hostname != NULL && portname != NULL){
      
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
//         
//         gnutls_anon_client_credentials_t anoncred;
//         gnutls_anon_allocate_client_credentials (&anoncred);
        
        gnutls_set_default_priority (session);
//         const int kx_prio[] = { GNUTLS_KX_ANON_DH, GNUTLS_KX_DHE_DSS, GNUTLS_KX_DHE_RSA, GNUTLS_KX_RSA_EXPORT, 0 };
//         gnutls_kx_set_priority (session, kx_prio);
//         gnutls_credentials_set (session, GNUTLS_CRD_ANON, anoncred);

#ifdef HAVE_IPV6
      
      struct addrinfo hints, *res, *ressave;
      int n;
      
      memset(&hints, 0, sizeof(struct addrinfo));
      
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      
      n = getaddrinfo(hostname, portname, &hints, &res);
      
      if (n <0) {
	fprintf(stderr,
		"getaddrinfo error:: [%s]\n",
		gai_strerror(n));
	return false;
      }
      
      ressave = res;
      
      sockfd=-1;
      while (res) {
	sockfd = socket(res->ai_family,
			res->ai_socktype,
			res->ai_protocol);
	
	if (!(sockfd < 0)) {
	  if (::connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
	    break;
	  
	  close(sockfd);
	  sockfd=-1;
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
      
      if ( pse = getservbyname(portname, "tcp") ) {
	sin.sin_port = pse->s_port;
	
      } else if ((sin.sin_port = htons((u_short)atoi(service)))==0) {
	fprintf(stderr, "ipv4_only_connect:: could not get service=[%s]\n",
		service);
	return false;
      }
      
      if (phe = gethostbyname(hostname)) {
	memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	
      } else if ( (sin.sin_addr.s_addr = inet_addr(hostname)) == 
		  INADDR_NONE) {
	fprintf(stderr, "ipv4_only_connect:: could not get host=[%s]\n", hostname);
	return false;
      }
      
      if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
	fprintf(stderr, "ipv4_only_connect:: could not open socket\n");
	return false;
      }
      
      if (connect(sockfd,(struct sockaddr *)&sin, sizeof(sin)) < 0) {
	fprintf(stderr, "ipv4_only_connect:: could not connect to host=[%s]\n", hostname);
	return false;
      }
      
#endif
      
      if(!(sockfd < 0)){
        gnutls_transport_set_ptr (session, (gnutls_transport_ptr_t) sockfd);
        int ret = gnutls_handshake (session);

        if (ret < 0)
        {
            fprintf (stderr, "*** Handshake failed\n");
            gnutls_perror (ret);
            close(sockfd);
            sockfd = 0;
            gnutls_deinit (session);
            return false;
        }
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
        close(sockfd);
        gnutls_deinit (session);
        sockfd = 0;
    }
    status = 0;
  }

  /*! \brief Sends a header and data to the socket.

  Sends the frame in two sends, one for the header and one for the data.
  Saves copying the header and data into another buffer.
  \param header The header data.
  \param hlen The length of the header data.
  \param data The data.
  \param len The length of the data.
  */
  void TlsSocket::send(char* header, int hlen, char* data, int len){
    if(status == 1){
      gnutls_record_send(session, header, hlen);
      gnutls_record_send(session, data, len);
    }
  }

  /*! \brief Receives a header from the network.

  Identical to recvBody().  If nothing is recieved from the network,
  we have been disconnected.
    \param len The length of the header to receive.
    \param data The array to store the data in.
    \return The length written into the data array.
    */
  int TlsSocket::recvHeader(int len, char* &data){
    if(status == 1){
      data = (char*)malloc(len);
      int rlen = gnutls_record_recv(session, data, len);
      if(rlen == 0)
	status = 0;
      return rlen;
    }
    return 0;
  }

  /*! \brief Receives data from the network.

  Identical to recvHeader().  If nothing is recieved from the network,
  we have been disconnected.
    \param len The length of the data to receive.
    \param data The array to store the data in.
    \return The length written into the data array.
    */
  int TlsSocket::recvBody(int len, char* &data){
    if(status == 1){
      data = (char*)malloc(len);
      int rlen = gnutls_record_recv(session, data, len);
      if(rlen == 0)
	status = 0;
      return rlen;
    }
    return 0;
  }

  /*! \brief Polls the network for data.

  Polls the network using select with zero timeout.  It does actually work.
  \return True if data is waiting, false otherwise.
  */
  bool TlsSocket::poll(){
    fd_set readfds;
    struct timeval timeout;

    // just poll, don't wait
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    return (select(sockfd + 1, &readfds, NULL, NULL, &timeout) == 1);

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

}
