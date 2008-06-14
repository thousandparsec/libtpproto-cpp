#ifndef TPPROTO_LOGMESSAGE_H
#define TPPROTO_LOGMESSAGE_H
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


/*! \file
  \brief Declares the LogMessage Admin Frame.
*/

#include <string>
#include <stdint.h>
#include <tpproto/frame.h>

namespace TPProto{
  
  /*! \brief The enum of levels for LogMessage Admin Frames.
  */
  enum LogMessageLevels {
    LML_Debug = 0,
    LML_Info = 1,
    LML_Warning = 2,
    LML_Error = 3
  };

  /*! \brief Log message.
   */
  class LogMessage : public Frame{
  public:
    LogMessage();
    virtual ~LogMessage();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    uint64_t getTimestamp();
    uint32_t getLevel();
    std::string getMessage();

  private:
    uint64_t timestamp;
    uint32_t level;
    std::string message;

  };

}

#endif
