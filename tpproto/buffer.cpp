
#include <iostream>
#include <netinet/in.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "buffer.h"

#ifdef htonll
# define htonq  htonll
# define ntohq  ntohll
#endif

#ifndef htonq
#ifndef WORDS_BIGENDIAN
# define htonq(i) ( ((long long)(htonl((i) & 0xffffffff)) << 32) | htonl(((i) >> 32) & 0xffffffff ) )
#else
#define htonq(i)        i
#endif 
# define ntohq  htonq
#endif

namespace TPProto{

  Buffer::Buffer(){
    data = NULL;
    datalen = 0;
    dataptr = 0;
  }

  Buffer::~Buffer(){
    if(data != NULL)
      free(data);
  }

  void Buffer::packInt(int val){
    int netval = htonl(val);
    data = (char *) realloc(data, datalen + 4);
    
    char* temp = data + datalen;
    
    memcpy(temp, &netval, 4);
    datalen += 4;

  }

  void Buffer::packInt64(long long val){
    long long netval = htonq(val);
    char *temp = (char *) realloc(data, datalen + 8);
    
    data = temp;
    temp += datalen;
    
    memcpy(temp, &netval, 8);
    datalen += 8;
  }

  void Buffer::packString(const char* val){
    int slen = strlen(val) + 1;
    
    int netlen = htonl(slen);
    char *temp = (char *) realloc(data, datalen + 4 + slen);
    
    data = temp;
    temp += datalen;
    
    // Length
    memcpy(temp, &netlen, 4);
    temp += 4;
    
    // Actual string
    memcpy(temp, val, slen);
    temp += slen;
    
    
    datalen += 4 + slen;
  }
  
  int Buffer::unpackInt(){
    int nval;
    memcpy(&nval, data + dataptr, 4);
    dataptr += 4;
    return ntohl(nval);
  }

  long long Buffer::unpackInt64(){
    long long nval;
    memcpy(&nval, data + dataptr, 8);
    dataptr += 8;
    return ntohq(nval);
  }

  char* Buffer::unpackString(){
    int len = unpackInt();
    char *rtnstr = NULL;
    
    if (len > 0 && datalen >= dataptr + len) {
      rtnstr = new char[len];
      memcpy(rtnstr, data + dataptr, len);
      
      dataptr += len;
    } else {
      //Logger::getLogger()->debug("len < 0 or length < upackptr + len");
      std::cout << "Buffer::unpackString(): len < 0 or length < upackptr + len" << std::endl;
    }
    //printf("unpackptr %d\n", unpackptr);
    return rtnstr;
  }

  int Buffer::peekInt(int offset){
    int nval;
    memcpy(&nval, data + offset, 4);
    return ntohl(nval);
  }

  void Buffer::createHeader(int ver, int seqnum, int type, int len){
    if(data != NULL)
      free(data);
    datalen = 16;
    dataptr = 0;
    data = (char*) malloc(datalen);
    data[0] = 'T';
    data[1] = 'P';
    data[2] = ver / 10 + '0';
    data[3] = ver % 10 + '0';
    int temp = htonl(seqnum);
    memcpy(data + 4, &temp, 4);
    temp = htonl(type);
    memcpy(data + 8, &temp, 4);
    temp = htonl(len);
    memcpy(data + 12, &temp, 4);
  }

  bool Buffer::readHeader(int &ver, int &seqnum, int &type, int &len){
    if(data == NULL || data[0] != 'T' || data[1] != 'P'){
      return false;
    }
    ver = (data[2] - '0') * 10 + data[3] - '0';
    int temp;
    memcpy(&temp, data + 4, 4);
    seqnum = ntohl(temp);
    memcpy(&temp, data + 8, 4);
    type = ntohl(temp);
    memcpy(&temp, data + 12, 4);
    len = ntohl(temp);
    return true;
  }

  void Buffer::setData(char* buff, int len){
    if(data != NULL)
      free(data);
    data = buff;
    datalen = len;
    dataptr = 0;
  }

  char* Buffer::getData(){
    return data;
  }

  int Buffer::getLength(){
    return datalen;
  }

}
