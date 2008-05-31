/*  ResourceListObjectParameter - Resource List ObjectParameters.
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

#include "resourcelistobjectparam.h"

namespace TPProto{
    
    ResourceListObjectParam::ResourceListObjectParam() : ObjectParameter(), reslist(){
    }
    
    ResourceListObjectParam::ResourceListObjectParam(const ResourceListObjectParam& rhs) :
            ObjectParameter(rhs), reslist(rhs.reslist){
    }
    
    ResourceListObjectParam::~ResourceListObjectParam(){
    }
    
    void ResourceListObjectParam::packBuffer(Buffer* buf){
        buf->packInt(0);
    }
    
    bool ResourceListObjectParam::unpackBuffer(Buffer* buf){
        int numres = buf->unpackInt();
        for(int i = 0; i < numres; i++){
            uint32_t resid = buf->unpackInt();
            ResourceCount rescount;
            rescount.stored = buf->unpackInt();
            rescount.minable = buf->unpackInt();
            rescount.unavailable = buf->unpackInt();
            reslist[resid] = rescount;
        }
        return true;
    }
    
    bool ResourceListObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    ObjectParameter* ResourceListObjectParam::clone(){
        return new ResourceListObjectParam(*this);
    }
    
    void ResourceListObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    std::map<uint32_t, ResourceCount> ResourceListObjectParam::getResourceList() const{
        return reslist;
    }
}
