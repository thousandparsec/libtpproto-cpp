#ifndef TPPROTO_GETTIME_H
#define TPPROTO_GETTIME_H
/*  FinishedFrame - Frame to tell the server that the player is finished his/her turn
 *
 *  Copyright (C) 2008  Lee Begg and the Thousand Parsec Project
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
  \brief Declares FinishedFrame Frame.
*/

#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief A Frame for tell the server that the player has finished his/her turn.
   */
  class FinsihedFrame : public Frame{
  public:
    FinishedFrame();
    virtual ~FinishedFrame();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    
  };

}

#endif
