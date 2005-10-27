/*  ModifyDesign frame class
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

#include "modifydesign.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    ModifyDesign::ModifyDesign() : Frame(), designid(0), categories(), name(), description(), owner(0xffffffff), 
                        components(){
    }

    /*! \brief Required virtual destructor.
    */
    ModifyDesign::~ModifyDesign(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this ModifyDesign frame into the buffer.
    \param buf Buffer to pack into.
    */
    void ModifyDesign::packBuffer(Buffer *buf){
        type = ft03_Design_Modify;
        buf->packInt(designid);
        buf->packInt64(0);
        buf->packInt(categories.size());
        for(std::set<uint32_t>::iterator i = categories.begin(); i != categories.end() ; ++i){
            buf->packInt(*i);
        }
        buf->packString(name.c_str());
        buf->packString(description.c_str());
        buf->packInt(0);
        buf->packInt(owner);
        buf->packInt(components.size());
        for(std::map<uint32_t, uint32_t>::iterator i = components.begin(); i != components.end(); ++i){
            buf->packInt(i->first);
            buf->packInt(i->second);
        }
        buf->packString("");
        buf->packInt(0);
    }

    /*! \brief Unpack from a Buffer, fails.
        Unpacks the ModifyDesign frame from the buffer. This method
        always fails and returns false, because the client cannot receive
        ModifyDesign frames form the server.
    \param buf The Buffer to unpack out of, ignored.
    \return False always.
    */
    bool ModifyDesign::unpackBuffer(Buffer *buf){
        return false;
    }

    /*! \brief Sets the Design id.
    \param did The Design id number.
    */
    void ModifyDesign::setDesignId(uint32_t did){
        designid = did;
    }

    /*! \brief Sets the categories this Design is in.
    \param cats The set of categories.
    */
    void ModifyDesign::setCategories(std::set<uint32_t> cats){
        categories = cats;
    }

    /*! \brief Sets the name of this Design.
    \param n The name of the Design.
    */
    void ModifyDesign::setName(const std::string& n){
        name = n;
    }

    /*! \brief Sets the description of this Design.
    \param d The description of the Design.
    */
    void ModifyDesign::setDescription(const std::string& d){
        description = d;
    }

    /*! \brief Sets the Player that owns this Design.
    Should be the current player's id.
    \param player The Player id of the owner.
    */
    void ModifyDesign::setPlayerOwner(uint32_t player){
        owner = player;
    }

    /*! \brief Sets the components that make up this design.
    \param comps The map of component ids and the number in this design.
    */
    void ModifyDesign::setComponents(std::map<uint32_t, uint32_t> comps){
        components = comps;
    }

}
