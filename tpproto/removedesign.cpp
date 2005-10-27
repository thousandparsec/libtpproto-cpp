/*  RemoveDesign class
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

#include "removedesign.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    RemoveDesign::RemoveDesign(){
    }

    /*! \brief Destructor.
    */
    RemoveDesign::~RemoveDesign(){
        idset.clear();
    }

    /*! \brief Pack this RemoveDesign Frame into a Buffer.
        \param buf The Buffer to pack into.
    */
    void RemoveDesign::packBuffer(Buffer* buf){
        buf->packInt(idset.size());
        for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
            buf->packInt(*itcurr);
        }

        type = ft03_Design_Remove;
    }

    /*! \brief Unpack from a Buffer, always false.

    Since RemoveDesign frames are only sent to the server, we should not receive them
    and therefore this method always returns false.
    \param buf The Buffer, ignored.
    \return False always.
    */
    bool RemoveDesign::unpackBuffer(Buffer* buf){
        return false;
    }

    /*! \brief Adds a single Design id to be removed.
        \param id The design id to remove.
    */
    void RemoveDesign::removeDesignId(unsigned int id){
        idset.insert(id);
    }

    /*! \brief Adds a set of Design ids to the set to be removed.
        \param ids The set of design ids to remove.
    */
    void RemoveDesign::removeDesignIds(std::set<unsigned int> ids){
        for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
            idset.insert(*itcurr);
        }
    }

}

