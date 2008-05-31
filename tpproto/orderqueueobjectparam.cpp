/*  OrderQueueObjectParameter - OrderQueue ObjectParameters.
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

#include "orderqueueobjectparam.h"

namespace TPProto{
    
    OrderQueueObjectParam::OrderQueueObjectParam() : ObjectParameter(), maxslots(0), queueid(0), numorders(0), allowedorders(){
    }
    
    OrderQueueObjectParam::OrderQueueObjectParam(const OrderQueueObjectParam& rhs) :
            ObjectParameter(rhs), maxslots(rhs.maxslots), queueid(0), numorders(0), allowedorders(){
    }
    
    OrderQueueObjectParam::~OrderQueueObjectParam(){
    }
    
    void OrderQueueObjectParam::packBuffer(Buffer* buf){
        buf->packInt(0);
        buf->packInt(0);
        buf->packInt(0);
    }
    
    bool OrderQueueObjectParam::unpackBuffer(Buffer* buf){
        queueid = buf->unpackInt();
        numorders = buf->unpackInt();
        int numallowed = buf->unpackInt();
        for(int i = 0; i < numallowed; i++){
            allowedorders.insert(buf->unpackInt());
        }
        return true;
    }
    
    bool OrderQueueObjectParam::unpackDescBuffer(Buffer* buf){
        maxslots = buf->unpackInt();
        return true;
    }
    
    ObjectParameter* OrderQueueObjectParam::clone(){
        return new OrderQueueObjectParam(*this);
    }
    
    void OrderQueueObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    uint32_t OrderQueueObjectParam::getMaxSlots() const{
        return maxslots;
    }
    
    uint32_t OrderQueueObjectParam::getQueueId() const{
        return queueid;
    }
    
    uint32_t OrderQueueObjectParam::getNumOrders() const{
        return numorders;
    }
    
    std::set<uint32_t> OrderQueueObjectParam::getAllowedOrderTypes() const{
        return allowedorders;
    }
}
