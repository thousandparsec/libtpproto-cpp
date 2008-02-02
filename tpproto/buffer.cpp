/*  Buffer - A buffer for frames to be packed into and out of and send on the network.
 *
 *  Copyright (C) 2005, 2006, 2008  Lee Begg and the Thousand Parsec Project
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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
    stringpadding = false;
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
  void Buffer::packInt(uint32_t val){
    uint32_t netval = htonl(val);
    data = (char *) realloc(data, datalen + 4);
    
    char* temp = data + datalen;
    
    memcpy(temp, &netval, 4);
    datalen += 4;

  }

  /*! \brief Packs a 64 bit int into the buffer.
      \param val The value to be packed.
  */
  void Buffer::packInt64(uint64_t val){
    uint64_t netval = htonq(val);
    char *temp = (char *) realloc(data, datalen + 8);
    
    data = temp;
    temp += datalen;
    
    memcpy(temp, &netval, 8);
    datalen += 8;
  }

  /*! \brief Packs a string into the buffer.
      \param val The string to be packed.
  */
  void Buffer::packString(const std::string& val){
    int slen = val.length();
    
    int netlen = htonl(slen);
    
    uint32_t pad = 0;
    if(stringpadding){
      pad = 4 - (slen % 4);
      if(pad == 4)
        pad = 0;
    }
    
    char *temp = (char *) realloc(data, datalen + 4 + slen + pad);
    
    data = temp;
    temp += datalen;
    
    // Length
    memcpy(temp, &netlen, 4);
    temp += 4;
    
    // Actual string
    memcpy(temp, val.c_str(), slen);
    temp += slen;
    
    
    for(uint32_t i = 0; i < pad; i++){
      temp[i] = 0;
    }
    
    datalen += 4 + slen + pad;
  }
  
  /*! \brief Unpacks a 32 bit int from the buffer.
      \return The value unpacked.
  */
  uint32_t Buffer::unpackInt(){
    uint32_t nval;
    memcpy(&nval, data + dataptr, 4);
    dataptr += 4;
    return ntohl(nval);
  }

  /*! \brief Unpacks a 64 bit int from the buffer.
      \return The value unpacked.
  */
  uint64_t Buffer::unpackInt64(){
    uint64_t nval;
    memcpy(&nval, data + dataptr, 8);
    dataptr += 8;
    return ntohq(nval);
  }
  
  /*! \brief Unpacks a string from the buffer.
      \return The string unpacked.
  */
  std::string Buffer::unpackString(){
    uint32_t len = unpackInt();
    
    uint32_t pad = 0;
    if(stringpadding){
      pad = 4 - (len % 4);
      if(pad == 4)
        pad = 0;
    }
    
    if (len > 0 && datalen >= dataptr + len + pad) {
      std::string rtnstr(data+dataptr, len);
      dataptr += len + pad;
      return rtnstr;
    } else {
      //Logger::getLogger()->debug("len < 0 or length < upackptr + len");
      std::cerr << "Buffer::unpackString(): len < 0 or length < upackptr + len + pad" << std::endl;
      std::cout << "len: " << len << " pad: " << pad << " length: " << datalen << " upackptr: " << dataptr << std::endl;
      throw new std::exception();
    }
    //printf("unpackptr %d\n", unpackptr);
    return std::string();
  }

  /*! \brief Peeks at the value of the 32 bit int at an offset into the
      buffer.

      This operation does not change the r/w position.
      \param offset The offset into the buffer.
      \return The 32 bit int value at the offset.
  */
  uint32_t Buffer::peekInt(uint32_t offset){
    uint32_t nval;
    memcpy(&nval, data + offset, 4);
    return ntohl(nval);
  }

  /*! \brief Creates a header for the given parameters.

    The buffer is set to the header.
    \param ver The version of the protocol.
    \param seqnum The sequence number.
    \param type The type number.
    \param len The length of the data of the frame in bytes.
    \param fver The frame type version, defaults to 0
  */
  void Buffer::createHeader(uint32_t ver, uint32_t seqnum, uint32_t type, uint32_t len, uint32_t fver){
    if(data != NULL)
      free(data);
    datalen = 16;
    dataptr = 0;
    data = (char*) malloc(datalen);
    data[0] = 'T';
    data[1] = 'P';
    if(ver < 4){
      data[2] = ver / 10 + '0';
      data[3] = ver % 10 + '0';
    }else{
      data[2] = ver;
      data[3] = fver;
    }
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
    \param len A reference to where the length of the data of the frame 
    in bytes should be stored.
    \param fver A reference to where the frame type version will be stored.
    \return True if the header is valid.
  */
  bool Buffer::readHeader(uint32_t &ver, uint32_t &seqnum, uint32_t &type, uint32_t &len, uint32_t &fver){
    if(data == NULL || data[0] != 'T' || data[1] != 'P'){
      return false;
    }
    if(data[2] == '0'){
      ver = (data[2] - '0') * 10 + data[3] - '0';
    }else{
      ver = data[2];
      fver = data[3];
    }
    uint32_t temp;
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
  void Buffer::setData(char* buff, uint32_t len){
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
  uint32_t Buffer::getLength(){
    return datalen;
  }

  /*! \brief Sets if String Padding is turned on.
  
  Should only be called by FrameCodec for use in the String Padding
  filter.
  \param nsp The new bool value for String Padding.
  */
  void Buffer::setStringPadding(bool nsp){
    stringpadding = nsp;
  }

  /*! \brief Checks if String Padding is turned on.
  \return True if string padding is no, false otherwise.
  */
  bool Buffer::getStringPadding() const{
    return stringpadding;
  }
  
}
