/*  OkFrame - Frame send by the server to tell the client that an operation was ok.
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

#include "okframe.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  OkFrame::OkFrame(){
    info = "";
  }

  /*! \brief Required virtual destructor.
   */
  OkFrame::~OkFrame(){

  }

  /*! \brief Pack into a Buffer, Assert(0).
    
  This Frame does not get sent to the server, so this method asserts zero.
  \param buf Buffer to pack into, ignored.
  */ 
  void OkFrame::packBuffer(Buffer *buf){
    // should never send an OK frame
    assert(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack out of.
    \return True if successful, false otherwise.
  */
  bool OkFrame::unpackBuffer(Buffer *buf){
    info = buf->unpackString();
    
    type = ft02_OK;

    return true;
  }

  /*! \brief Gets the informational string sent in the OKFrame.
    \return The informational string.
  */
  std::string OkFrame::getInfo(){
    return info;
  }

}
