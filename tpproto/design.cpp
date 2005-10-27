/*  Design frame class
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

#include "design.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    Design::Design() : Frame(), designid(0), modtime(0), categories(), name(), description(), inuse(0xffffffff), owner(0xffffffff), 
                        components(), feedback(), propvalues(){
    }

    /*! \brief Required virtual destructor.
    */
    Design::~Design(){
    }

    /*! \brief Pack into a Buffer, asserts.
    
    Packs this Design frame into the buffer. Since the frame only
    comes from the server, this method assert(0).
    \param buf Buffer to pack into, ignored.
    */ 
    void Design::packBuffer(Buffer *buf){
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the Design frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool Design::unpackBuffer(Buffer *buf){
        designid = buf->unpackInt();
        modtime = buf->unpackInt64();
        uint32_t numcat = buf->unpackInt();
        for(uint32_t i = 0; i < numcat; i++){
            categories.insert(buf->unpackInt());
        }
        name = buf->unpackString();
        description = buf->unpackString();
        inuse = buf->unpackInt();
        owner = buf->unpackInt();
        uint32_t numcomp = buf->unpackInt();
        for(uint32_t i = 0; i < numcomp; i++){
            uint32_t id = buf->unpackInt();
            components[id] = buf->unpackInt();
        }
        feedback = buf->unpackString();
        uint32_t numprop = buf->unpackInt();
        for(uint32_t i = 0; i < numprop; i++){
            uint32_t id = buf->unpackInt();
            propvalues[id] =buf->unpackString();
        }
        type = ft03_Design;
        return true;
    }

    /*! \brief Gets the Design id.
    \return The Design id number.
    */
    uint32_t Design::getDesignId() const{
        return designid;
    }

    /*! \brief Gets the last modification time of this Design.
    \return The modtime.
    */
    uint64_t Design::getModTime() const{
        return modtime;
    }

    /*! \brief Gets the categories this Design is in.
    \return The set of categories.
    */
    std::set<uint32_t> Design::getCategories() const{
        return categories;
    }

    /*! \brief Gets the name of this Design.
    \return The name of the Design.
    */
    std::string Design::getName() const{
        return name;
    }

    /*! \brief Gets the description of this Design.
    \return The description of the Design.
    */
    std::string Design::getDescription() const{
        return description;
    }

    /*! \brief Gets the number of times this design is in use.
    If is 0xffffffff (ie, -1), then the design is not currently usable.
    \return The number of times the design is in use.
    */
    uint32_t Design::getNumInUse() const{
        return inuse;
    }

    /*! \brief Gets the Player that owns this Design.
    Can be 0xffffffff (ie, -1) if the owner is not known.
    \return The Player id of the owner.
    */
    uint32_t Design::getPlayerOwner() const{
        return owner;
    }

    /*! \brief Gets the components that make up this design.
    \return The map of component ids and the number in this design.
    */
    std::map<uint32_t, uint32_t> Design::getComponents() const{
        return components;
    }

    /*! \brief Gets the feedback on the design.
    \return The feedback.
    */
    std::string Design::getDesignFeedback() const{
        return feedback;
    }

    /*! \brief Gets the property values of this design.
    \return The map of property value id and string display value.
    */
    std::map<uint32_t, std::string> Design::getPropertyValues() const{
        return propvalues;
    }

}
