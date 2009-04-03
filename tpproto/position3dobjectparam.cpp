/*  Position3dObjectParameter - Position3d ObjectParameters.
 *
 *  Copyright (C) 2008, 2009  Lee Begg and the Thousand Parsec Project
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
#include "objectparametervisitor.h"

#include "position3dobjectparam.h"

namespace TPProto{
    
    /*! Default constructor.
    */
    Position3dObjectParam::Position3dObjectParam() : ObjectParameter(), pos(), relative(0){
    }
    
    /*! Constructs a copy of the given Position3dObjectParam.
    \param rhs The Position3dObjectParam to copy.
    */
    Position3dObjectParam::Position3dObjectParam(const Position3dObjectParam& rhs) :
            ObjectParameter(rhs), pos(rhs.pos), relative(rhs.relative){
    }
    
    /*! Destructor.
    */
    Position3dObjectParam::~Position3dObjectParam(){
    }
    
    /*! \brief Pack this Position3dObjectParam into the Buffer.
    
    Packs all zeros because all fields are readonly and the client can't modify them.
    \param buf The Buffer to pack.
    */
    void Position3dObjectParam::packBuffer(Buffer* buf){
        buf->packInt64(0);
        buf->packInt64(0);
        buf->packInt64(0);
        buf->packInt(0);
    }
    
    /*! \brief Unpack this Position3dObjectParam from the Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
    */
    bool Position3dObjectParam::unpackBuffer(Buffer* buf){
        pos.unpack(buf);
        relative = buf->unpackInt();
        return true;
    }
    
    /*! \brief Unpack the description of this Position3dObjectParam.
    The description is empty, so do nothing.
    \param buf The Buffer to unpack from.
    \return True if successful.
    */
    bool Position3dObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    /*! \brief Create a copy of this Position3dObjectParam.
    \return A new copy of this Position3dObjectParam.
    */
    ObjectParameter* Position3dObjectParam::clone(){
        return new Position3dObjectParam(*this);
    }
    
    /*! \brief Let the ObjectParameterVisitor visit this ObjectParameter.
    \param opv The ObjectParameterVisitor that is visiting this parameter.
    */
    void Position3dObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    /*! \brief Get the position of the Object.
    \return The position in a Vector3d.
    */
    Vector3d Position3dObjectParam::getPosition() const{
        return pos;
    }
    
    /*! \brief Get the ObjectId of the Object this position is relative to.
    \return The ObjectId of the relative object.
    */
    uint32_t Position3dObjectParam::getRelativeObject() const{
        return relative;
    }
}
