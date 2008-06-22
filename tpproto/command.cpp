/*  Command Admin Frame
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

#include "buffer.h"
#include "commandparameter.h"
#include "commanddesc.h"

#include "command.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    Command::Command()
    {
    }

    /*! \brief Destructor.

    Deletes the CommandParameters it contains.
    */
    Command::~Command()
    {
        for(std::list<CommandParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr)
            delete *itcurr;
        params.clear();
    }

    /*! \brief Pack this Command Frame into a Buffer.
        \param buf The Buffer to pack into.
    */
    void Command::packBuffer(Buffer* buf)
    {
        buf->packInt(desc->getCommandType());

        //pack paramters
        for(std::list<CommandParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr)
            (*itcurr)->packBuffer(buf);

        type = ftad_Command;
    }

    /*! \brief Unpack from a Buffer, fails always.
        Command frames are only sent to the server, so unpacking is not needed.
    \param buf The Buffer to unpack out of, ignored.
    \return False always.
    */
    bool Command::unpackBuffer(Buffer* buf)
    {
        return false;
    }

    /*! \brief Gets the name of this command type.
        \return The name of the command.
    */
    std::string Command::getName()
    {
        return desc->getName();
    }

    /*! \brief Gets the text description of this command type.
        \return The text description of the command.
    */
    std::string Command::getDescription()
    {
        return desc->getDescription();
    }

    /*! \brief Gets the command type number of this command.
        \return The command type number.
    */
    unsigned int Command::getCommandType()
    {
        return desc->getCommandType();
    }

    /*! \brief Gets the number of CommandParameters this command has.
        \return The number of parameters.
    */
    unsigned int Command::getNumParameters()
    {
        return params.size();
    }

    /*! \brief Gets a CommandParameter.
        \param which The number of the parameter to get.
        \return The which-th CommandParameter on this order or NULL if which is not valid.
    */
    CommandParameter* Command::getParameter(unsigned int which)
    {
        if(which < params.size()){
            std::list<CommandParameter*>::iterator itcurr = params.begin();
            advance(itcurr, which);
            if(itcurr != params.end())
	            return *itcurr;
        }

        return NULL;
    }

    /*! \brief Sets the command type using a CommandDescription.
        \param cd The CommandDescription describing the command type required.
    */
    void Command::setCommandType(boost::shared_ptr<CommandDescription> cd)
    {
        desc = cd;
        params = cd->getParameters();
    }

}

