/*  GetObjectIdsByPos frame class
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

#include "getobjectidsbypos.h"

namespace TPProto{

    /*! \brief Default constructor.
    
    Sets the position to 0,0,0 (centre of the universe) and the radius to 0.
    */
    GetObjectIdsByPos::GetObjectIdsByPos(){
        pos = Vector3d(0ll, 0ll, 0ll);
        rad = 0ll;
        type = ft03_ObjectIds_GetByPos;
    }

    /*! \brief Required virtual destructor.
    */
    GetObjectIdsByPos::~GetObjectIdsByPos(){
    
    }
    
    /*! \brief Pack the Frame into a Buffer.
        \param buf The Buffer to pack into.
    */
    void GetObjectIdsByPos::packBuffer(Buffer* buf){
        pos.pack(buf);
        buf->packInt64(rad);
    }

    /*! \brief Unpack Buffer, always false.
        
    This Frame should never be received, therefore this method always 
    returns false.
    \return False, always.
    */
    bool GetObjectIdsByPos::unpackBuffer(Buffer* buf){
        return false;
    }

    /*! \brief Sets the position for the centre of the area to be looked at.
        \param npos The position to be taken as the centre.
    */
    void GetObjectIdsByPos::setPosition(const Vector3d &npos){
        pos = npos;
    }

    /*! \brief Sets the radius for the area to be looked at.
        \param nrad The radius to be used.
    */
    void GetObjectIdsByPos::setRadius(uint64_t nrad){
        rad = nrad;
    }

}
