/*  CommandDescription Admin Frame
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
#include "commandparameter.h"

#include "commanddesc.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    CommandDescription::CommandDescription()
    {
    }

    /*! \brief Destructor.

    Deletes the CommandParameters it contains.
    */
    CommandDescription::~CommandDescription()
    {
        for(std::list<CommandParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr)
            delete *itcurr;
        params.clear();
    }

    /*! \brief Pack into a Buffer, assert(0).

    This Frame is only received from the server and therefore does not get
    packed on the client side.  assert(0) is called.
        \param buf The Buffer, ignored.
    */
    void CommandDescription::packBuffer(Buffer* buf)
    {
        assert(0);
    }

    /*! \brief Unpack this CommandDescription from a Buffer.
        \param buf The Buffer to unpack out of.
        \return True if successful.
    */
    bool CommandDescription::unpackBuffer(Buffer* buf)
    {
        ctype = buf->unpackInt();
        name = buf->unpackString();
        desc = buf->unpackString();

        //unpack args
        int numparams = buf->unpackInt();

        for(int i = 0; i < numparams; i++)
        {
            std::string temp = buf->unpackString();
            int ptype = buf->unpackInt();
            CommandParameter* opm = NULL;
            switch(ptype)
            {
            case cpT_String:
                opm = new StringCommandParameter();
                break;
            case cpT_Integer:
                opm = new IntegerCommandParameter();
                break;
            default:
	            break;
            }
            if(opm != NULL)
	            opm->setName(temp);

            temp = buf->unpackString();
            if(opm != NULL)
            {
	            opm->setDescription(temp);
	            params.push_back(opm);
            }
        }
        modtime = buf->unpackInt64();
    
        type = ftad_CommandDesc;

        return true;
    }

    /*! \brief Gets the command type number of this command description.
        \return The command type number.
    */
    unsigned int CommandDescription::getCommandType()
    {
        return ctype;
    }

    /*! \brief Gets the name of this command description.
        \return The name of the command description.
    */
    std::string CommandDescription::getName()
    {
        return name;
    }

    /*! \brief Gets the text description of this command description.
        \return The text description of the command description.
    */
    std::string CommandDescription::getDescription()
    {
        return desc;
    }

    /*! \brief Gets a copy of the CommandParameters in this command description.
        \return A copy of the list of CommandParameters.
    */
    std::list<CommandParameter*> CommandDescription::getParameters()
    {
        std::list<CommandParameter*> np;
        for(std::list<CommandParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr)
            np.push_back((*itcurr)->clone());
        return np;
    }

    /*! \brief Returns the timestamp of the last time this order description was modified.
        \return The timestamp of the modification time.
    */
    uint64_t CommandDescription::getLastModifiedTime()
    {
        return modtime;
    }

}

