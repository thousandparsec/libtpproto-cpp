#ifndef TPPROTO_BUFFER_H
#define TPPROTO_BUFFER_H
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

/*! \file buffer.h
  \brief Declares Buffer class.
  
  The Buffer class is a buffer.
*/

#include <string>
#include <stdint.h>

namespace TPProto{
  
  /*! \brief Buffer for packing and unpacking Frames

  Supports unpacking and packing various base types and creating
  and reading the frame header.
  */
  class Buffer{
  public:

    Buffer();
    ~Buffer();

    void packInt(uint32_t val);
    void packInt64(uint64_t val);
    void packString(const std::string& val);

    uint32_t unpackInt();
    uint64_t unpackInt64();
    std::string unpackString();

    uint32_t peekInt(uint32_t offset);

    void createHeader(uint32_t ver, uint32_t seqnum, uint32_t type, uint32_t len, uint32_t fver = 0);
    bool readHeader(uint32_t &ver, uint32_t &seqnum, uint32_t &type, uint32_t &len, uint32_t &fver);

    void setData(char* buff, uint32_t len);
    char* getData();
    uint32_t getLength();
    
    void setStringPadding(bool nsp);
    bool getStringPadding() const;

  private:
    char* data;
    uint32_t datalen;
    uint32_t dataptr;
    bool stringpadding;

  };

}

#endif
