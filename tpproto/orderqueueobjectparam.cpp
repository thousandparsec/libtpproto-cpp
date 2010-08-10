/*  OrderQueueObjectParameter - OrderQueue ObjectParameters.
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

#include "orderqueueobjectparam.h"

namespace TPProto{
    
    /*! Constructor.
    */
    OrderQueueObjectParam::OrderQueueObjectParam() : ObjectParameter(), queueid(0){
    }
    
    /*! Copy construct the OrderQueueObjectParam from another one
    \param rhs The OrderQueueObjectParam to copy.
    */
    OrderQueueObjectParam::OrderQueueObjectParam(const OrderQueueObjectParam& rhs) :
            ObjectParameter(rhs), queueid(0){
    }
    
    /*! Destructor.
    */
    OrderQueueObjectParam::~OrderQueueObjectParam(){
    }
    
    /*! \brief Pack into a Buffer.
    
    Packs only zeros as all fields are read only.
    \param buf The Buffer to pack into.
    */
    void OrderQueueObjectParam::packBuffer(Buffer* buf){
        buf->packInt(0);
    }
    
    /*! \brief Unpack this OrderQueueObjectParam from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
    */
    bool OrderQueueObjectParam::unpackBuffer(Buffer* buf){
        queueid = buf->unpackInt();
        return true;
    }
    
    /*! \brief Unpack the Description Buffer.
    
    Unpacks the maximum number of order slots in the OrderQueue for this
    ObjectType.
    \param buf The Buffer to unpack from.
    \return True if successful.
    */
    bool OrderQueueObjectParam::unpackDescBuffer(Buffer* buf){
        return true;
    }
    
    /*! \brief Clone this OrderQueueObjectParam.
    \return A new copy of this OrderQueueObjectParam.
    */
    ObjectParameter* OrderQueueObjectParam::clone(){
        return new OrderQueueObjectParam(*this);
    }
    
    /*! \brief Have a Visitor visit this object.
    \param opv The ObjectParameterVisitor that is visiting.
    */
    void OrderQueueObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    /*! \brief Get the Queue Id for this OrderQueue.
    \return The OrderQueue Id.
    */
    uint32_t OrderQueueObjectParam::getQueueId() const{
        return queueid;
    }
    
}
