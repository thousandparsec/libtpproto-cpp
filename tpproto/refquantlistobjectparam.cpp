/*  RefQuantityListObjectParameter - Reference Quantity List ObjectParameters.
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

#include "refquantlistobjectparam.h"

namespace TPProto{
    
    /*! \brief Constructor.
    */
    RefQuantityListObjectParam::RefQuantityListObjectParam() : ObjectParameter(), list(){
    }
    
    /*! \brief Copy construct from another RefQuantityListObjectParam.
    \param rhs The RefQuantityList to copy.
    */
    RefQuantityListObjectParam::RefQuantityListObjectParam(const RefQuantityListObjectParam& rhs) :
            ObjectParameter(rhs), list(rhs.list){
    }
    
    /*! \brief Destructor.
    */
    RefQuantityListObjectParam::~RefQuantityListObjectParam(){
    }
    
    /*! \brief Pack into a Buffer.
    
    Packs a zero, as all fields are read-only.
    \param buf The Buffer to pack into.
    */
    void RefQuantityListObjectParam::packBuffer(Buffer* buf){
        buf->packInt(0);
    }
    
    /*! \brief Unpack this RefQuantityListObjectParam from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
    */
    bool RefQuantityListObjectParam::unpackBuffer(Buffer* buf){
        int numitems = buf->unpackInt();
        
        for(int i = 0; i < numitems; i++){
            int32_t reftype = buf->unpackInt();
            uint32_t refid = buf->unpackInt();
            uint32_t quant = buf->unpackInt();
            list[RefTypeAndId(reftype, refid)] = quant;
        }
        
        return true;
    }
    
    /*! \brief Unpack the Description from a Buffer.
    
    Does not unpack anything.
    \param buf The Buffer to unpack from.
    \return True.
    */
    bool RefQuantityListObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    /*! \brief Create a clone of this RefQuantityListObjectParam.
    \return A new copy of this RefQuantityListObjectParam.
    */
    ObjectParameter* RefQuantityListObjectParam::clone(){
        return new RefQuantityListObjectParam(*this);
    }
    
    /*! \brief Have an ObjectParameterVisitor to visit.
    \param opv The ObjectParameter that is visiting.
    */
    void RefQuantityListObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    /*! \brief Get the Reference quantity list.
    \return The RefQuantityList.
    */
    RefQuantityListObjectParam::RefQuantityList RefQuantityListObjectParam::getRefQuantityList() const{
        return list;
    }
}
