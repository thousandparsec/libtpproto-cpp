/*  GetCommandTypesList Admin Frame
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

#include "getcommandtypeslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetCommandTypesList::GetCommandTypesList() : GetIdSequence()
    {
    }

    /*! \brief Required virtual destructor.
    */
    GetCommandTypesList::~GetCommandTypesList()
    {
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetOrderTypesList frame into the buffer.
        \param buf Buffer to pack into.
    */ 
    void GetCommandTypesList::packBuffer(Buffer *buf)
    {
        GetIdSequence::packBuffer(buf);
        type = ftad_CommandTypes_Get;
    }

}
