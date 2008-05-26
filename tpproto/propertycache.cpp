/*  PropertyCache - Cache of Properties class
 *
 *  Copyright (C) 2006, 2008  Lee Begg and the Thousand Parsec Project
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

#include <list>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cachemethod.h"
#include "property.h"
#include "getproperty.h"
#include "getpropertyidslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "propertycache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    PropertyCache::PropertyCache() : Cache(), watchers(), waiters(){
    }

    /*! \brief Destructor.
    */
    PropertyCache::~PropertyCache(){
    }

    void PropertyCache::requestProperty(uint32_t propid, const PropertyCallback &cb){
        PropertySignal* bs = waiters[propid];
        if(bs == NULL){
            bs = new PropertySignal();
            waiters[propid] = bs;
        }
        bs->connect(cb);
        cache->getById(propid);
    }
    
    boost::signals::connection PropertyCache::watchProperty(uint32_t propid, const PropertyCallback &cb){
        PropertySignal *bs = watchers[propid];
        if(bs == NULL){
            bs = new PropertySignal();
            watchers[propid] = bs;
        }
        boost::signals::connection conn = bs->connect(cb);
        requestProperty(propid, cb);
        return conn;
    }
    
    void PropertyCache::requestPropertyIds(const IdSetCallback& cb){
        cache->getAllIds(cb);
    }
    
    boost::signals::connection PropertyCache::watchPropertyIds(const IdSetCallback& cb){
        return cache->watchAllIds(cb);
    }
    
    GetIdSequence* PropertyCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetPropertyIdsList();
    }

    GetById* PropertyCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetProperty();
    }

    uint32_t PropertyCache::getIdFromFrame(Frame* frame){
        Property* prop = dynamic_cast<Property*>(frame);
        if(prop != NULL && prop->getType() == ft03_Property){
            return prop->getPropertyId();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t PropertyCache::getModTimeFromFrame(Frame* frame){
        Property* prop = dynamic_cast<Property*>(frame);
        if(prop != NULL && prop->getType() == ft03_Property){
            return prop->getModTime();
        }else{
            return 0LL;
        }
    }

    void PropertyCache::newItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Property> property(boost::dynamic_pointer_cast<Property>(item));
        if(property){
            PropertySignal* bs = waiters[property->getPropertyId()];
            if(bs != NULL){
                (*bs)(property);
                delete bs;
            }
            waiters.erase(property->getPropertyId());
            bs = watchers[property->getPropertyId()];
            if(bs != NULL){
                (*bs)(property);
            }
        }
        
    }
    
    void PropertyCache::existingItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Property> property(boost::dynamic_pointer_cast<Property>(item));
        if(property){
            PropertySignal* bs = waiters[property->getPropertyId()];
            if(bs != NULL){
                (*bs)(property);
                delete bs;
            }
            waiters.erase(property->getPropertyId());
        }
    }
    
}
