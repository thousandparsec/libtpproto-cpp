/*  Property frame class
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

#include <cassert>

#include "buffer.h"

#include "property.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    Property::Property() : Frame(), propid(0), modtime(0), categories(), rank(0), name(), displayname(), description(),
                tpcl_calculate(), tpcl_requires(){
    }

    /*! \brief Required virtual destructor.
    */
    Property::~Property(){
    }

    /*! \brief Pack into a Buffer, asserts.
    
    Packs this Property frame into the buffer. Since the frame only
    comes from the server, this method assert(0).
    \param buf Buffer to pack into, ignored.
    */ 
    void Property::packBuffer(Buffer *buf){
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the Property frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool Property::unpackBuffer(Buffer *buf){
        propid = buf->unpackInt();
        modtime = buf->unpackInt64();
        uint32_t numcat = buf->unpackInt();
        for(uint32_t i = 0; i < numcat; i++){
            categories.insert(buf->unpackInt());
        }
        rank = buf->unpackInt();
        name = buf->unpackString();
        displayname = buf->unpackString();
        description = buf->unpackString();
        tpcl_calculate = buf->unpackString();
        tpcl_requires = buf->unpackString();
        type = ft03_Property;
        return true;
    }

    /*! \brief Gets the Property id.
    \return The Property id number.
    */
    uint32_t Property::getPropertyId() const{
        return propid;
    }

    /*! \brief Gets the last modification time of this Property.
    \return The modtime.
    */
    uint64_t Property::getModTime() const{
        return modtime;
    }

    /*! \brief Gets the categories this Property is in.
        \return The set of categories.
    */
    std::set<uint32_t> Property::getCategories() const{
        return categories;
    }

    /*! \brief Gets the rank of this property.
    \return The rank number.
    */
    uint32_t Property::getRank() const{
        return rank;
    }

    /*! \brief Gets the name of this Property.
    This is a valid tpcl name. For a name that can be displayed, look at the display name.
    \return The name of the Property.
    */
    std::string Property::getName() const{
        return name;
    }

    /*! \brief Gets the display name of this Property.
    For a name that can be used in TPCL, see the name.
    \return The display name of the Property.
    */
    std::string Property::getDisplayName() const{
        return displayname;
    }

    /*! \brief Gets the description of this Property.
    \return The description of the Property.
    */
    std::string Property::getDescription() const{
        return description;
    }

    /*! \brief Gets the TPCL calulate function for this Property.
    \return The TPCL calculate function for the Property.
    */
    std::string Property::getTpclCalculateFunc() const{
        return tpcl_calculate;
    }

    /*! \brief Gets the TPCL requirements function of this Property.
    \return The TPCL requirements function for the Property.
    */
    std::string Property::getTpclRequirementsFunc() const{
        return tpcl_requires;
    }

}
