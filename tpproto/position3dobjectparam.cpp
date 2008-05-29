/*  Position3dObjectParameter - Position3d ObjectParameters.
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

#include "position3dobjectparam.h"

namespace TPProto{
    
    Position3dObjectParam::Position3dObjectParam() : ObjectParameter(), pos(), relative(0){
    }
    
    Position3dObjectParam::Position3dObjectParam(const Position3dObjectParam& rhs) :
            ObjectParameter(rhs), pos(rhs.pos), relative(rhs.relative){
    }
    
    Position3dObjectParam::~Position3dObjectParam(){
    }
    
    void Position3dObjectParam::packBuffer(Buffer* buf){
        buf->packInt64(0);
        buf->packInt64(0);
        buf->packInt64(0);
        buf->packInt(0);
    }
    
    bool Position3dObjectParam::unpackBuffer(Buffer* buf){
        pos.unpack(buf);
        relative = buf->unpackInt();
        return true;
    }
    
    bool Position3dObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    ObjectParameter* Position3dObjectParam::clone(){
        return new Position3dObjectParam(*this);
    }
    
    void Position3dObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
            
    Vector3d Position3dObjectParam::getPosition() const{
        return pos;
    }
    
    uint32_t Position3dObjectParam::getRelativeObject() const{
        return relative;
    }
}
