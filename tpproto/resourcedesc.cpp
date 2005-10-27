/*  ResourceDescription frame class
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

#include "resourcedesc.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    ResourceDescription::ResourceDescription() : Frame(), resid(0), namesing(), nameplur(), unitsing(),
                unitplur(), desc(), mass(0), size(0), modtime(0){
    }

    /*! \brief Required virtual destructor.
    */
    ResourceDescription::~ResourceDescription(){
    }

    /*! \brief Pack into a Buffer, asserts.
    
    Packs this ResourceDescription frame into the buffer. Since the frame only
    comes from the server, this method assert(0).
    \param buf Buffer to pack into, ignored.
    */ 
    void ResourceDescription::packBuffer(Buffer *buf){
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the ResourceDescription frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool ResourceDescription::unpackBuffer(Buffer *buf){
        resid = buf->unpackInt();
        namesing = buf->unpackString();
        nameplur = buf->unpackString();
        unitsing = buf->unpackString();
        unitplur = buf->unpackString();
        desc = buf->unpackString();
        mass = buf->unpackInt();
        size = buf->unpackInt();
        if(protoVer >= 3)
            modtime = buf->unpackInt64();
        type = ft02_ResDesc;
        return true;
    }

    /*! \brief Gets the Resource type.
    \return The resource type number.
    */
    uint32_t ResourceDescription::getResourceType() const{
        return resid;
    }
    
    /*! \brief Gets the name of the resource in singular form.
        The name is available in singular and in plural.
    \return The name in singular form.
    */
    std::string ResourceDescription::getSingularName() const{
        return namesing;
    }
    
    /*! \brief Gets the name of the resource in plural form.
        The name is available in singular and in plural.
        \return The name in plural form.
     */
    std::string ResourceDescription::getPluralName() const{
        return nameplur;
    }
    
    /*! \brief Gets the name of the units of the resource in singular form.
        The name of the units is available in singular and in plural.
    \return The name of the units in singular form.
     */
    std::string ResourceDescription::getSingularUnit() const{
        return unitsing;
    }
    
    /*! \brief Gets the name of the units of the resource in plural form.
        The name of the units is available in singular and in plural.
    \return The name of the units in plural form.
     */
    std::string ResourceDescription::getPluralUnit() const{
        return unitplur;
    }

    /*! \brief Gets the description of the resource.
        \return The description.
     */
    std::string ResourceDescription::getDescription() const{
        return desc;
    }
    
    /*! \brief Gets the mass of one unit of the resource.
        Can be 0 if not applicable. Don't know what units the mass is in.
    \return The mass of one unit.
     */
    uint32_t ResourceDescription::getWeight() const{
        return mass;
    }

     /*! \brief Gets the size of one unit of the resource.
        Can be 0 if not applicable. Don't know what units the size is in.
        \return The size of one unit.
      */
    uint32_t ResourceDescription::getSize() const{
        return size;
    }

    /*! \brief Gets the last modification time of this resource description.
    \return The modtime.
    */
    uint64_t ResourceDescription::getModTime() const{
        return modtime;
    }

}
