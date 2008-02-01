/*  FinishedFrame - tells server player has finished turn
 *
 *  Copyright (C) 2008 Lee Begg and the Thousand Parsec Project
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


#include "buffer.h"

#include "finished.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  FinishedFrame::FinishedFrame(){
  }

  /*! \brief Required virtual destructor.
   */
  FinishedFrame::~FinishedFrame(){
  }

  /*! \brief Pack this FinishedFrame Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void FinishedFrame::packBuffer(Buffer* buf){
    type = ft04_TurnFinished;
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since FinishedFrame frames are only sent to the server, we should not 
  receive them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool FinishedFrame::unpackBuffer(Buffer* buf){
    return false;
  }

}

