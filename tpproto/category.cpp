/*  Category frame class
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

#include "category.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
Category::Category() : Frame(), catid(0), modtime(0), name(), description(){
    }

    /*! \brief Required virtual destructor.
    */
    Category::~Category(){
    }

    /*! \brief Pack into a Buffer, asserts.
    
    Packs this Category frame into the buffer. Since the frame only
    comes from the server, this method assert(0).
    \param buf Buffer to pack into, ignored.
    */ 
    void Category::packBuffer(Buffer *buf){
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the Category frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool Category::unpackBuffer(Buffer *buf){
        catid = buf->unpackInt();
        modtime = buf->unpackInt64();
        name = buf->unpackString();
        description = buf->unpackString();
        type = ft03_Category;
        return true;
    }

    /*! \brief Gets the Category id.
    \return The Category id number.
    */
    uint32_t Category::getCategoryId() const{
        return catid;
    }

    /*! \brief Gets the last modification time of this Category.
    \return The modtime.
    */
    uint64_t Category::getModTime() const{
        return modtime;
    }

    /*! \brief Gets the name of this Category.
    \return The name of the Category.
    */
    std::string Category::getName() const{
        return name;
    }

    /*! \brief Gets the description of this Category.
    \return The description of the Category.
    */
    std::string Category::getDescription() const{
        return description;
    }

}
