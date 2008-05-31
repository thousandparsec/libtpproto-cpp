
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tcpsocket.h"

namespace TPProto{

  /*! \brief Default constructor.

  Sets up defaults, that will prevent connecting.
  */
  TcpSocket::TcpSocket() : TPSocket(){
    status = 0;
    hostname = NULL;
    portname = NULL;
  }

  /*! \brief Destructor.
   */
  TcpSocket::~TcpSocket(){
    if(hostname != NULL)
      delete[] hostname;
    if(portname != NULL)
      delete[] portname;
  }

  /*! \brief Check if the socket is connected.
    \return True if connected, false otherwise.
  */
  bool TcpSocket::isConnected(){
    return status == 1;
  }

  /*! \brief Connects to the host and port over TCP.
    
  If HAVE_IPV6 is defined, it will use either IPv4 or IPv6 to
  make the connection, otherwise it will only use IPv4 (old APIs). They follow 
  different code paths.
  \return True if a new connection is made, false if already connected or
  if an error occured.
  */
  bool TcpSocket::connect(){
    if(status == 0 && hostname != NULL && portname != NULL){
      
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
      
      if ( pse = getservbyname(portname, "tcp") ) {
	sin.sin_port = pse->s_port;
	
      } else if ((sin.sin_port = htons((u_short)atoi(portname)))==0) {
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
      
      if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
	fprintf(stderr, "ipv4_only_connect:: could not open socket\n");
	return false;
      }
      
      if (connect(fd,(struct sockaddr *)&sin, sizeof(sin)) < 0) {
	fprintf(stderr, "ipv4_only_connect:: could not connect to host=[%s]\n", hostname);
	return false;
      }
      
#endif
      
      if(!(fd < 0)){
	status = 1;
        fcntl(fd, F_SETFL, O_NONBLOCK);
	return true;
      }
    }
    
    
    return false;
  }

  /*! \brief Closes the connection.

  The connection is closed after calling this method (even if it was already
  closed).
  */
  void TcpSocket::disconnect(){
    if(status == 1){
      close(fd);
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
    int TcpSocket::send(const char* data, int len){
        if(status != 1){
            throw new DisconnectedException();
        }
        int slen = ::send(fd, data, len, 0);
       
        if(slen < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            return 0;
         if(slen <= 0){
            status = 0;
            close(fd);
            throw new DisconnectedException();
        }
        
        return slen;
    }

 

  /*! \brief Receives data from the network.

  If nothing is recieved from the network,
  we have been disconnected, and throws a DisconnectedException.
    \throw DisconnectionException if not connected.
    \param len The length of the data to receive.
    \param data The array to store the data in.
    \return The length written into the data array.
    */
  int TcpSocket::recv(int len, char* data){
        if(status != 1){
            throw new DisconnectedException();
        }
        
        int rlen = ::recv(fd, data, len, 0);
       
        if(rlen < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            return 0;
         if(rlen <= 0){
            status = 0;
            close(fd);
            throw new DisconnectedException();
        }
        return rlen;
    
  }

  /*! \brief Sets the server address and port to connect to.

  Host can be DNS name or ip address. Post can be port name or number.
  \param host Host to connect to.
  \param port Port to connect to.
  */
  void TcpSocket::setServerAddr(const char* host, const char* port){
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
