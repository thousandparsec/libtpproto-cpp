/*  ResourceListObjectParameter - Resource List ObjectParameters.
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

#include "resourcelistobjectparam.h"

namespace TPProto{
    /*! Constructs an empty ResourceListObjectParam.
    */
    ResourceListObjectParam::ResourceListObjectParam() : ObjectParameter(), reslist(){
    }
    
    /*! Copy constructs a ResourceListObjectParam from another one.
    \param rhs The ResouceListObjectParam to copy.
    */
    ResourceListObjectParam::ResourceListObjectParam(const ResourceListObjectParam& rhs) :
            ObjectParameter(rhs), reslist(rhs.reslist){
    }
    
    /*! Destructor.
    */
    ResourceListObjectParam::~ResourceListObjectParam(){
    }
    
    /*! \brief Pack this ResourceListObjectParam into the Buffer.
    
    Packs a single uint32 as an empty list, as the whole list is read only.
    \param buf The Buffer to pack into.
    */
    void ResourceListObjectParam::packBuffer(Buffer* buf){
        buf->packInt(0);
    }
    
    /*! \brief Unpack this ResourceListObjectParam from the Buffer.
    \param buf The Buffer to unpack.
    \return True if successful.
    */
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
    
    /*! \brief Unpack the description of this ResourceListObjectParam.
    
    No fields to unpack.
    \param buf The Buffer to unpack.
    \return True.
    */
    bool ResourceListObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    /*! \brief Clone this ResourceListObjectParam.
    \return A new copy of this ResourceListObjectParam.
    */
    ObjectParameter* ResourceListObjectParam::clone(){
        return new ResourceListObjectParam(*this);
    }
    
    /*! \brief Have an ObjectParameterVisitor visit this ResourceListObjectParam.
    \param opv The ObjectParameterVisitor that is visiting.
    */
    void ResourceListObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    /*! \brief Get the list of Resources.
    \return The Resource list as a map of Resource ID to ResourceCount.
    */
    std::map<uint32_t, ResourceCount> ResourceListObjectParam::getResourceList() const{
        return reslist;
    }
}
