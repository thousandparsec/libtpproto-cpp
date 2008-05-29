/*  SizeObjectParameter - Size ObjectParameters.
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

#include "sizeobjectparam.h"

namespace TPProto{
    
    SizeObjectParam::SizeObjectParam() : ObjectParameter(), size(0){
    }
    
    SizeObjectParam::SizeObjectParam(const SizeObjectParam& rhs) :
            ObjectParameter(rhs), size(rhs.size){
    }
    
    SizeObjectParam::~SizeObjectParam(){
    }
    
    void SizeObjectParam::packBuffer(Buffer* buf){
        buf->packInt64(0);
    }
    
    bool SizeObjectParam::unpackBuffer(Buffer* buf){
        size = buf->unpackInt64();
        return true;
    }
    
    bool SizeObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    ObjectParameter* SizeObjectParam::clone(){
        return new SizeObjectParam(*this);
    }
    
    void SizeObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    uint64_t SizeObjectParam::getSize() const{
        return size;
    }
}
