/*  GetResourceDescription class
 *
 *  Copyright (C) 2005  Lee Begg and the Thousand Parsec Project
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

#include "getresourcedesc.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetResourceDescription::GetResourceDescription(){
    }

    /*! \brief Destructor.
    */
    GetResourceDescription::~GetResourceDescription(){
        idset.clear();
    }

    /*! \brief Pack this GetResourceDescription Frame into a Buffer.
        \param buf The Buffer to pack into.
    */
    void GetResourceDescription::packBuffer(Buffer* buf){
        buf->packInt(idset.size());
        for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
            buf->packInt(*itcurr);
        }

        type = ft02_ResDesc_Get;
    }

    /*! \brief Unpack from a Buffer, always false.

    Since GetResourceDescription frames are only sent to the server, we should not receive them
    and therefore this method always returns false.
    \param buf The Buffer, ignored.
    \return False always.
    */
    bool GetResourceDescription::unpackBuffer(Buffer* buf){
        return false;
    }

    /*! \brief Adds a single Resource type to get.
        \param id The Resource type to get.
    */
    void GetResourceDescription::addResourceType(unsigned int id){
        idset.insert(id);
    }

    /*! \brief Adds a set of Resource types to the set to request.
        \param ids The set of Resource types to get.
    */
    void GetResourceDescription::addResouceTypes(std::set<unsigned int> ids){
        for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
            idset.insert(*itcurr);
        }
    }

}

