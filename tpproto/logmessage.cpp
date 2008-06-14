/*  LogMessage Admin Frame
 *
 *  Copyright (C) 2008 Aaron Mavrinac and the Thousand Parsec Project
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

#include "logmessage.h"

namespace TPProto{
 
  /*! \brief Default constructor.
   */
  LogMessage::LogMessage(){
  }
  
  /*! \brief Required virtual destructor.
   */
  LogMessage::~LogMessage(){
  }

  /*! \brief Pack into a Buffer, Assert(0).
    
  This Frame does not get sent to the server, so this method asserts zero.
  \param buf Buffer to pack into, ignored.
  */ 
  void LogMessage::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool LogMessage::unpackBuffer(Buffer* buf){
    timestamp = buf->unpackInt64();
    level = buf->unpackInt();
    message = buf->unpackString();
    type = ftad_LogMessage;
    return true;
  }

  /*! \brief Gets the timestamp for this message.
    \return The timestamp (in POSIX time).
  */
  uint64_t LogMessage::getTimestamp(){
    return timestamp;
  }

  /*! \brief Gets the severity level of this message.
    \return The severity level.
  */
  uint32_t LogMessage::getLevel(){
    return level;
  }

  /*! \brief Gets the log message string.
    \return The message string.
  */
  std::string LogMessage::getMessage(){
    return message;
  }

}

