/*  IntegerObjectParameter - Integer ObjectParameters.
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

#include "integerobjectparam.h"

namespace TPProto{
    
    /*! \brief Constructs an IntegerObjectParam.
    */
    IntegerObjectParam::IntegerObjectParam() : ObjectParameter(), value(0){
    }
    
    /*! \brief Constructs an IntegerObjectParam from another one.
    \param rhs The IntegerObjectParam to copy
    */
    IntegerObjectParam::IntegerObjectParam(const IntegerObjectParam& rhs) :
            ObjectParameter(rhs), value(rhs.value){
    }
    
    /*! Destructor.
    */
    IntegerObjectParam::~IntegerObjectParam(){
    }
    
    /*! \brief Pack into a Buffer.
    
    Always packs zero as it is always read only
    \param buf The Buffer to pack into.
    */
    void IntegerObjectParam::packBuffer(Buffer* buf){
        buf->packInt(0);
    }
    
    /*! \brief Unpack this IntegerObjectParam from a Buffer.
    \param buf The Butter to unpack from.
    \return True if successful.
    */
    bool IntegerObjectParam::unpackBuffer(Buffer* buf){
        value = buf->unpackInt();
        return true;
    }
    
    /*! \brief Unpack Description Buffer.
    
    Does nothing as no fields to unpack.
    \param buf The Buffer to unpack.
    \return True.
    */
    bool IntegerObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    /*! \brief Clone this IntegerObjectParam.
    \return A new copy of this IntegerObjectParam.
    */
    ObjectParameter* IntegerObjectParam::clone(){
        return new IntegerObjectParam(*this);
    }
    
    /*! \brief Visit this object with a Visitor
    \param opv The ObjectParameterVisitor that is visiting.
    */
    void IntegerObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    /*! \brief Gets the value of this IntegerObjectParam.
    \return The value.
    */
    uint32_t IntegerObjectParam::getValue() const{
        return value;
    }
}
