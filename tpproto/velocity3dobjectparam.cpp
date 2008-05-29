/*  Velocity3dObjectParameter - Velocity3d ObjectParameters.
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

#include "velocity3dobjectparam.h"

namespace TPProto{
    
    Velocity3dObjectParam::Velocity3dObjectParam() : ObjectParameter(), vel(), relative(0){
    }
    
    Velocity3dObjectParam::Velocity3dObjectParam(const Velocity3dObjectParam& rhs) :
            ObjectParameter(rhs), vel(rhs.vel), relative(rhs.relative){
    }
    
    Velocity3dObjectParam::~Velocity3dObjectParam(){
    }
    
    void Velocity3dObjectParam::packBuffer(Buffer* buf){
        buf->packInt64(0);
        buf->packInt64(0);
        buf->packInt64(0);
        buf->packInt(0);
    }
    
    bool Velocity3dObjectParam::unpackBuffer(Buffer* buf){
        vel.unpack(buf);
        relative = buf->unpackInt();
        return true;
    }
    
    bool Velocity3dObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    ObjectParameter* Velocity3dObjectParam::clone(){
        return new Velocity3dObjectParam(*this);
    }
    
    void Velocity3dObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
            
    Vector3d Velocity3dObjectParam::getVelocity() const{
        return vel;
    }
    
    uint32_t Velocity3dObjectParam::getRelativeObject() const{
        return relative;
    }
}
