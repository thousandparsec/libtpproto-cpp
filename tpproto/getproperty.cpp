/*  GetProperty class
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

#include "getproperty.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetProperty::GetProperty(){
    }

    /*! \brief Destructor.
    */
    GetProperty::~GetProperty(){
        idset.clear();
    }

    /*! \brief Pack this GetProperty Frame into a Buffer.
        \param buf The Buffer to pack into.
    */
    void GetProperty::packBuffer(Buffer* buf){
        buf->packInt(idset.size());
        for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
            buf->packInt(*itcurr);
        }

        type = ft03_Property_Get;
    }

    /*! \brief Unpack from a Buffer, always false.

    Since GetProperty frames are only sent to the server, we should not receive them
    and therefore this method always returns false.
    \param buf The Buffer, ignored.
    \return False always.
    */
    bool GetProperty::unpackBuffer(Buffer* buf){
        return false;
    }

    /*! \brief Adds a single Property id to get.
        \param id The property id to get.
    */
    void GetProperty::addPropertyId(unsigned int id){
        idset.insert(id);
    }

    /*! \brief Adds a set of Property ids to the set to request.
        \param ids The set of property ids to get.
    */
    void GetProperty::addPropertyIds(std::set<unsigned int> ids){
        for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
            idset.insert(*itcurr);
        }
    }

}

