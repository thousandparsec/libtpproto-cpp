/*  SizeObjectParameter - Size ObjectParameters.
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

#include "sizeobjectparam.h"

namespace TPProto{
    
    /*! Construct SizeObjectParam.
    */
    SizeObjectParam::SizeObjectParam() : ObjectParameter(), size(0){
    }
    
    /*! Construct a SizeObjectParam from another one.
    \param rhs The SizeObjectParam to copy.
    */
    SizeObjectParam::SizeObjectParam(const SizeObjectParam& rhs) :
            ObjectParameter(rhs), size(rhs.size){
    }
    
    /*! Destructor
    */
    SizeObjectParam::~SizeObjectParam(){
    }
    
    /*! \brief Pack this SizeObjectParam into the Buffer.
    
    Packs zero into the Buffer because the field is read only.
    \param buf The Buffer to pack into.
    */
    void SizeObjectParam::packBuffer(Buffer* buf){
        buf->packInt64(0);
    }
    
    /*! \brief Unpack this SizeObjectParam from the Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
    */
    bool SizeObjectParam::unpackBuffer(Buffer* buf){
        size = buf->unpackInt64();
        return true;
    }
    
    /*! \brief Unpack the description of this SizeObjectParam.
    
    No fields to unpack.
    \param buf The Buffer to unpack.
    \return True.
    */
    bool SizeObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    /*! \brief Clone this SizeObjectParam.
    \return A new copy of this SizeObjectParam.
    */
    ObjectParameter* SizeObjectParam::clone(){
        return new SizeObjectParam(*this);
    }
    
    /*! \brief Have an ObjectParameterVisitor visit.
    \param opv The ObjectParameterVisitor that is visiting.
    */
    void SizeObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    /*! \brief Get the Size of the object.
    \return The size.
    */
    uint64_t SizeObjectParam::getSize() const{
        return size;
    }
}
