#ifndef TPPROTO_TIMEREMAINING_H
#define TPPROTO_TIMEREMAINING_H
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


/*! \file
  \brief Declares the TimeRemaining Frame.
*/

#include <stdint.h>
#include <string>

#include <tpproto/frame.h>

namespace TPProto{
  
  /*! \brief The enum of known reason codes for TimeRemaining Frames.
  */
  enum TimeRemainingReasons {
    TRR_Requested = 0,
    TRR_Timer_Started = 1,
    TRR_Advanced_Warning = 2,
    TRR_All_Finished_EOT = 3,
    TRR_Threshold_Finished = 4,
    TRR_EOT_Started = 5
  };

  /*! \brief Notification of the time remaining before the end of turn.
   */
  class TimeRemaining : public Frame{
  public:
    TimeRemaining();
    virtual ~TimeRemaining();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    uint32_t getTimeRemaining();
    uint32_t getReasonCode();
    uint32_t getTurnNumber();
    std::string getTurnName();

  private:
    uint32_t time;
    uint32_t reason;
    uint32_t turnnum;
    std::string turnname;

  };

}

#endif
