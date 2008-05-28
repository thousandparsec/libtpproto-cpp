/*  OrderDescCache - Cache of OrderDescriptions class
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cachemethod.h"
#include "orderdesc.h"
#include "getorderdesc.h"
#include "getordertypeslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "orderdesccache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    OrderDescCache::OrderDescCache() : Cache(){
    }

    /*! \brief Destructor.
    */
    OrderDescCache::~OrderDescCache(){
    }

    void OrderDescCache::requestOrderDescription(uint32_t ordtype, const OrderDescCallback &cb){
        OrderDescSignal* bs = waiters[ordtype];
        if(bs == NULL){
            bs = new OrderDescSignal();
            waiters[ordtype] = bs;
        }
        bs->connect(cb);
        cache->getById(ordtype);
    }
    
    boost::signals::connection OrderDescCache::watchOrderDescription(uint32_t ordtype, const OrderDescCallback &cb){
        OrderDescSignal *bs = watchers[ordtype];
        if(bs == NULL){
            bs = new OrderDescSignal();
            watchers[ordtype] = bs;
        }
        boost::signals::connection conn = bs->connect(cb);
        requestOrderDescription(ordtype, cb);
        return conn;
    }
    
    void OrderDescCache::requestOrderTypes(const IdSetCallback& cb){
        cache->getAllIds(cb);
    }
    
    boost::signals::connection OrderDescCache::watchOrderTypes(const IdSetCallback& cb){
        return cache->watchAllIds(cb);
    }
    
    
    GetIdSequence* OrderDescCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetOrderTypesList();
    }

    GetById* OrderDescCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetOrderDescription();
    }

    uint32_t OrderDescCache::getIdFromFrame(Frame* frame){
        OrderDescription* ord = dynamic_cast<OrderDescription*>(frame);
        if(ord != NULL && ord->getType() == ft02_OrderDesc){
            return ord->getOrderType();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t OrderDescCache::getModTimeFromFrame(Frame* frame){
        OrderDescription* ord = dynamic_cast<OrderDescription*>(frame);
        if(ord != NULL && ord->getType() == ft02_OrderDesc){
            return ord->getLastModifiedTime();
        }else{
            return 0LL;
        }
    }
    
    void OrderDescCache::newItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<OrderDescription> orddesc(boost::dynamic_pointer_cast<OrderDescription>(item));
        if(orddesc){
            OrderDescSignal* bs = waiters[orddesc->getOrderType()];
            if(bs != NULL){
                (*bs)(orddesc);
                delete bs;
            }
            waiters.erase(orddesc->getOrderType());
            bs = watchers[orddesc->getOrderType()];
            if(bs != NULL){
                (*bs)(orddesc);
            }
        }
        
    }
    
    void OrderDescCache::existingItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<OrderDescription> orddesc(boost::dynamic_pointer_cast<OrderDescription>(item));
        if(orddesc){
            OrderDescSignal* bs = waiters[orddesc->getOrderType()];
            if(bs != NULL){
                (*bs)(orddesc);
                delete bs;
            }
            waiters.erase(orddesc->getOrderType());
        }
    }

}
