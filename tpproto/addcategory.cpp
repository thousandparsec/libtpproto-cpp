/*  AddCategory frame class
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

#include "addcategory.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    AddCategory::AddCategory() : Frame(), name(), description(){
    }

    /*! \brief Required virtual destructor.
    */
    AddCategory::~AddCategory(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this AddCategory frame into the buffer.
    \param buf Buffer to pack into.
    */
    void AddCategory::packBuffer(Buffer *buf){
        type = ft03_Category_Add;
        buf->packInt(0);
        buf->packInt64(0);
        buf->packString(name.c_str());
        buf->packString(description.c_str());
    }

    /*! \brief Unpack from a Buffer, fails.
        Unpacks the AddCategory frame from the buffer. Since these
        frames are always sent to the server, this method always fails
        and returns false.
    \param buf The Buffer to unpack out of, ignored.
    \return False always.
    */
    bool AddCategory::unpackBuffer(Buffer *buf){
        return false;
    }

    /*! \brief Sets the name of this Category.
    \param n The name of the Category.
    */
    void AddCategory::setName(const std::string& n){
        name = n;
    }

    /*! \brief Sets the description of this Category.
    \param d The description of the Category.
    */
    void AddCategory::setDescription(const std::string& d){
        description = d;
    }

}
