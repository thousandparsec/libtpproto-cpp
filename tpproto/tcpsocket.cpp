
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tcpsocket.h"

namespace TPProto{

  TcpSocket::TcpSocket(){
    status = 0;
    sockfd = -1;
    hostname = NULL;
    portname = NULL;
  }

  TcpSocket::~TcpSocket(){
    if(hostname != NULL)
      delete hostname;
    if(portname != NULL)
      delete portname;
  }

  bool TcpSocket::isConnected(){
    return status == 1;
  }

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
	status = 1;
	return true;
      }
    }
    
    
    return false;
  }

  void TcpSocket::disconnect(){
    if(status == 1){
      close(sockfd);
      sockfd = 0;
    }
    status = 0;
  }

  void TcpSocket::send(char* header, int hlen, char* data, int len){
    if(status == 1){
      ::send(sockfd, header, hlen, 0);
      ::send(sockfd, data, len, 0);
    }
  }

  int TcpSocket::recvHeader(int len, char* &data){
    if(status == 1){
      data = new char[len];
      int rlen = ::recv(sockfd, data, len, 0);
      if(rlen == 0)
	status = 0;
      return rlen;
    }
    return 0;
  }
  
  int TcpSocket::recvBody(int len, char* &data){
    if(status == 1){
      data = new char[len];
      int rlen = ::recv(sockfd, data, len, 0);
      if(rlen == 0)
	status = 0;
      return rlen;
    }
    return 0;
  }

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