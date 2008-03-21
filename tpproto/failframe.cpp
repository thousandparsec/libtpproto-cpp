/*  FailFrame - Frame send by the server to tell the client that an operation failed.
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
#include <cassert>

#include "buffer.h"

#include "failframe.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  FailFrame::FailFrame() : errcode(0), errstring(), refs(){
  }

  /*! \brief Required virtual destructor.
   */
  FailFrame::~FailFrame(){

  }

  /*! \brief Assets(0).
    
  This method asserts 0 because it should never be sent to the server.
  \param buf The Buffer (ignored).
  */
  void FailFrame::packBuffer(Buffer *buf){
    // should never send an Fail frame
    assert(0);
  }

  /*! \brief Unpack FailFrame from Buffer.
    
  Unpacks the FrameErrorCode and error string from the buffer.
  \param buf The Buffer to unpack from.
  \return True if successful.
  */
  bool FailFrame::unpackBuffer(Buffer *buf){
    errcode = buf->unpackInt();
    errstring = buf->unpackString();
    
    if(protoVer >= 4){
      int numref = buf->unpackInt();
      for(int i = 0; i < numref; i++){
        int32_t reftype = buf->unpackInt();
        uint32_t refval = buf->unpackInt();
        refs[reftype] = refval;
      }
    }
    
    type = ft02_Fail;

    return true;
  }

  /*! \brief Gets the error code.
    \return The error code.
  */
  int FailFrame::getErrorCode(){
    return errcode;
  }

  /*! \brief Gets the error string.
    \return The error string.
  */
  std::string FailFrame::getErrorString(){
    return errstring;
  }
  
  std::map<int32_t, uint32_t> FailFrame::getReferences(){
    return refs;
  }

}
