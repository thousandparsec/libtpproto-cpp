/*  CommandTypesList frame class
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

#include "commandtypeslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    CommandTypesList::CommandTypesList() : IdSequence()
    {
    }

    /*! \brief Required virtual destructor.
    */
    CommandTypesList::~CommandTypesList()
    {
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the CommandTypesList frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool CommandTypesList::unpackBuffer(Buffer *buf)
    {
        IdSequence::unpackBuffer(buf);
        type = ftad_CommandTypes_List;
        return true;
    }

}
