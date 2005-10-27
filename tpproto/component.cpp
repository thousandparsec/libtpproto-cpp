/*  Component frame class
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

#include "component.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    Component::Component() : Frame(), compid(0), modtime(0), categories(), name(), description(),
            tpcl_requires(), tpcl_propvalues(){
    }

    /*! \brief Required virtual destructor.
    */
    Component::~Component(){
    }

    /*! \brief Pack into a Buffer, asserts.
    
    Packs this Component frame into the buffer. Since the frame only
    comes from the server, this method assert(0).
    \param buf Buffer to pack into, ignored.
    */ 
    void Component::packBuffer(Buffer *buf){
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the Component frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool Component::unpackBuffer(Buffer *buf){
        compid = buf->unpackInt();
        modtime = buf->unpackInt64();
        uint32_t numcat = buf->unpackInt();
        for(uint32_t i = 0; i < numcat; i++){
            categories.insert(buf->unpackInt());
        }
        name = buf->unpackString();
        description = buf->unpackString();
        tpcl_requires = buf->unpackString();
        uint32_t numprop = buf->unpackInt();
        for(uint32_t i = 0; i < numprop; i++){
            uint32_t id = buf->unpackInt();
            tpcl_propvalues[id] = buf->unpackString();
        }
        type = ft03_Component;
        return true;
    }

    /*! \brief Gets the Component id.
    \return The Component id number.
    */
    uint32_t Component::getComponentId() const{
        return compid;
    }

    /*! \brief Gets the last modification time of this Component.
    \return The modtime.
    */
    uint64_t Component::getModTime() const{
        return modtime;
    }

    /*! \brief Gets the categories this Component is in.
    \return The set of categories.
    */
    std::set<uint32_t> Component::getCategories() const{
        return categories;
    }

    /*! \brief Gets the name of this Component.
    \return The name of the Component.
    */
    std::string Component::getName() const{
        return name;
    }

    /*! \brief Gets the description of this Component.
    \return The description of the Component.
    */
    std::string Component::getDescription() const{
        return description;
    }

    /*! \brief Gets the TPCL Requirements function for this Component.
    \return The TPCL Requirements function for this Component.
    */
    std::string Component::getTpclRequirementsFunc() const{
        return tpcl_requires;
    }

    /*! \brief Gets the Property instances functions for this Component.
    \return The map of property id and tpcl value functions for the Component.
    */
    std::map<uint32_t, std::string> Component::getTpclPropertyValueFuncs() const{
        return tpcl_propvalues;
    }

}
