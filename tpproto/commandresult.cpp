/*  CommandResult Admin Frame
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

#include "commandresult.h"

namespace TPProto{
 
    /*! \brief Default constructor.
    */
    CommandResult::CommandResult()
    {
    }
  
    /*! \brief Required virtual destructor.
     */
    CommandResult::~CommandResult()
    {
    }

    /*! \brief Pack into a Buffer, Assert(0).
    
    This Frame does not get sent to the server, so this method asserts zero.
        \param buf Buffer to pack into, ignored.
    */ 
    void CommandResult::packBuffer(Buffer* buf)
    {
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        \param buf The Buffer to unpack from.
        \return True if successful.
    */
    bool CommandResult::unpackBuffer(Buffer* buf)
    {
        status = buf->unpackInt();
        message = buf->unpackString();
        type = ftad_CommandResult;
        return true;
    }

    /*! \brief Gets the return status of the command.
        \return The return status.
    */
    uint32_t CommandResult::getStatus()
    {
        return status;
    }

    /*! \brief Gets the command output message string.
        \return The output message string.
    */
    std::string CommandResult::getMessage()
    {
        return message;
    }

}

