/* ReferenceObjectParameter - Reference ObjectParameters.
 *
 *  Copyright (C) 2008  Lee Begg and the Thousand Parsec Project
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

#include "referenceobjectparam.h"

namespace TPProto{
    
    ReferenceObjectParam::ReferenceObjectParam() : ObjectParameter(), reftype(0), refid(0){
    }
    
    ReferenceObjectParam::ReferenceObjectParam(const ReferenceObjectParam& rhs) :
            ObjectParameter(rhs), reftype(rhs.reftype), refid(rhs.refid){
    }
    
    ReferenceObjectParam::~ReferenceObjectParam(){
    }
    
    void ReferenceObjectParam::packBuffer(Buffer* buf){
        buf->packInt(0);
        buf->packInt(0);
    }
    
    bool ReferenceObjectParam::unpackBuffer(Buffer* buf){
        reftype = buf->unpackInt();
        refid = buf->unpackInt();
        return true;
    }
    
    bool ReferenceObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    ObjectParameter* ReferenceObjectParam::clone(){
        return new ReferenceObjectParam(*this);
    }
    
    void ReferenceObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    int32_t ReferenceObjectParam::getRefType() const{
        return reftype;
    }
    
    uint32_t ReferenceObjectParam::getRefId() const{
        return refid;
    }
}
