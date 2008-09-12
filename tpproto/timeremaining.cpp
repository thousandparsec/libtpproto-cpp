/*  TimeRemaining Frame
 *
 *  Copyright (C) 2005-2006, 2008  Lee Begg and the Thousand Parsec Project
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

#include "timeremaining.h"

namespace TPProto{
 
  /*! \brief Default constructor.
   */
  TimeRemaining::TimeRemaining(){
  }
  
  /*! \brief Required virtual destructor.
   */
  TimeRemaining::~TimeRemaining(){
  }

  /*! \brief Pack into a Buffer, Assert(0).
    
  This Frame does not get sent to the server, so this method asserts zero.
  \param buf Buffer to pack into, ignored.
  */ 
  void TimeRemaining::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool TimeRemaining::unpackBuffer(Buffer* buf){
    time = buf->unpackInt();
    if(protoVer >= 4){
      reason = buf->unpackInt();
      turnnum = buf->unpackInt();
      turnname = buf->unpackString();
    }
    type = ft02_Time_Remaining;
    return true;
  }

  /*! \brief Gets the time remaining before the end of turn.
    \return The time in seconds before the end of turn.
  */
  uint32_t TimeRemaining::getTimeRemaining(){
    return time;
  }

  uint32_t TimeRemaining::getReasonCode(){
    return reason;
  }

  uint32_t TimeRemaining::getTurnNumber(){
      return turnnum;
  }
  
  std::string TimeRemaining::getTurnName(){
      return turnname;
  }
  
}

