/*  GetPlayerIdsList frame class
 *
 *  Copyright (C) 2009  Lee Begg and the Thousand Parsec Project
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

#include "getplayeridslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetPlayerIdsList::GetPlayerIdsList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetPlayerIdsList::~GetPlayerIdsList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetPlayerIdsList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetPlayerIdsList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft04_PlayerIds_Get;
    }


}
