
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

  /*! \brief Default constructor.
   */
  Buffer::Buffer(){
    data = NULL;
    datalen = 0;
    dataptr = 0;
  }

  /*! \brief Default destructor.
   */
  Buffer::~Buffer(){
    if(data != NULL)
      free(data);
  }

  /*! \brief Packs a 32 bit int into the buffer.
      \param val The value to be packed.
  */
  void Buffer::packInt(int val){
    int netval = htonl(val);
    data = (char *) realloc(data, datalen + 4);
    
    char* temp = data + datalen;
    
    memcpy(temp, &netval, 4);
    datalen += 4;

  }

  /*! \brief Packs a 64 bit int into the buffer.
      \param val The value to be packed.
  */
  void Buffer::packInt64(long long val){
    long long netval = htonq(val);
    char *temp = (char *) realloc(data, datalen + 8);
    
    data = temp;
    temp += datalen;
    
    memcpy(temp, &netval, 8);
    datalen += 8;
  }

  /*! \brief Packs a string into the buffer.
      \param val The string to be packed.
  */
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
  
  /*! \brief Unpacks a 32 bit int from the buffer.
      \return The value unpacked.
  */
  int Buffer::unpackInt(){
    int nval;
    memcpy(&nval, data + dataptr, 4);
    dataptr += 4;
    return ntohl(nval);
  }

  /*! \brief Unpacks a 64 bit int from the buffer.
      \return The value unpacked.
  */
  long long Buffer::unpackInt64(){
    long long nval;
    memcpy(&nval, data + dataptr, 8);
    dataptr += 8;
    return ntohq(nval);
  }
  
  /*! \brief Unpacks a string from the buffer.
      \return The string unpacked.
  */
  char* Buffer::unpackString(){
    int len = unpackInt();
    char *rtnstr = NULL;
    
    if (len > 0 && datalen >= dataptr + len) {
      rtnstr = new char[len + 1];
      memcpy(rtnstr, data + dataptr, len);
      data[len] = '\0';
      dataptr += len;
    } else {
      //Logger::getLogger()->debug("len < 0 or length < upackptr + len");
      std::cerr << "Buffer::unpackString(): len < 0 or length < upackptr + len" << std::endl;
      std::cout << "len: " << len << " length: " << datalen << " upackptr: " << dataptr << std::endl;
    }
    //printf("unpackptr %d\n", unpackptr);
    return rtnstr;
  }

  /*! \brief Peeks at the value of the 32 bit int at an offset into the
      buffer.

      This operation does not change the r/w position.
      \param offset The offset into the buffer.
      \return The 32 bit int value at the offset.
  */
  int Buffer::peekInt(int offset){
    int nval;
    memcpy(&nval, data + offset, 4);
    return ntohl(nval);
  }

  /*! \brief Creates a header for the given parameters.

    The buffer is set to the header.
    \param ver The version of the protocol.
    \param seqnum The sequence number.
    \param type The type number.
    \param len The length of the data of the frame in bytes.
  */
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

  /*! \brief Reads a header, passing back the parameters.

    Some checks are performed to check that the buffer is a valid
    header.
    \param ver A reference to where the version number of the protocol will
    be stored.
    \param seqnum A reference to where the sequence number will be stored.
    \param type A reference to where the type number will be stored.
    \param len A reference to where the lenght of the data of the frame 
    in bytes should be stored.
    \return True if the header is valid.
  */
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

  /*! \brief Sets the buffer with the given data.

    Should not be called in general, used by FrameCodec to set the data
    received from the network.
      \param buff The char array of data to be copied as the buffer.
      \param len The length of the array of data for the buffer.
  */
  void Buffer::setData(char* buff, int len){
    if(data != NULL)
      free(data);
    data = buff;
    datalen = len;
    dataptr = 0;
  }

  /*! \brief Gets the data in the buffer.

    Should not be called in general, used by FrameCodec to send the data
    on the network.
    \return Data in the buffer.
  */
  char* Buffer::getData(){
    return data;
  }

  /*! \brief Gets the current length of the data in the buffer.
      \return The length of the buffer in bytes.
  */
  int Buffer::getLength(){
    return datalen;
  }

}
