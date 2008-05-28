/*  ObjectDescCache - Cache of OrderDescriptions class
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
#include "objectdesc.h"
#include "getobjectdesc.h"
#include "getobjecttypeslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "objectdesccache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    ObjectDescCache::ObjectDescCache() : Cache(){
    }

    /*! \brief Destructor.
    */
    ObjectDescCache::~ObjectDescCache(){
    }

    void ObjectDescCache::requestObjectDescription(uint32_t objtype, const ObjectDescCallback &cb){
        ObjectDescSignal* bs = waiters[objtype];
        if(bs == NULL){
            bs = new ObjectDescSignal();
            waiters[objtype] = bs;
        }
        bs->connect(cb);
        cache->getById(objtype);
    }
    
    boost::signals::connection ObjectDescCache::watchObjectDescription(uint32_t objtype, const ObjectDescCallback &cb){
        ObjectDescSignal *bs = watchers[objtype];
        if(bs == NULL){
            bs = new ObjectDescSignal();
            watchers[objtype] = bs;
        }
        boost::signals::connection conn = bs->connect(cb);
        requestObjectDescription(objtype, cb);
        return conn;
    }
    
    void ObjectDescCache::requestObjectTypes(const IdSetCallback& cb){
        cache->getAllIds(cb);
    }
    
    boost::signals::connection ObjectDescCache::watchObjectTypes(const IdSetCallback& cb){
        return cache->watchAllIds(cb);
    }
    
    
    GetIdSequence* ObjectDescCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetObjectTypesList();
    }

    GetById* ObjectDescCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetObjectDescription();
    }

    uint32_t ObjectDescCache::getIdFromFrame(Frame* frame){
        ObjectDescription* obj = dynamic_cast<ObjectDescription*>(frame);
        if(obj != NULL && obj->getType() == ft04_ObjectDesc){
            return obj->getObjectType();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t ObjectDescCache::getModTimeFromFrame(Frame* frame){
        ObjectDescription* obj = dynamic_cast<ObjectDescription*>(frame);
        if(obj != NULL && obj->getType() == ft04_ObjectDesc){
            return obj->getLastModifiedTime();
        }else{
            return 0LL;
        }
    }
    
    void ObjectDescCache::newItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<ObjectDescription> objdesc(boost::dynamic_pointer_cast<ObjectDescription>(item));
        if(objdesc){
            ObjectDescSignal* bs = waiters[objdesc->getObjectType()];
            if(bs != NULL){
                (*bs)(objdesc);
                delete bs;
            }
            waiters.erase(objdesc->getObjectType());
            bs = watchers[objdesc->getObjectType()];
            if(bs != NULL){
                (*bs)(objdesc);
            }
        }
        
    }
    
    void ObjectDescCache::existingItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<ObjectDescription> objdesc(boost::dynamic_pointer_cast<ObjectDescription>(item));
        if(objdesc){
            ObjectDescSignal* bs = waiters[objdesc->getObjectType()];
            if(bs != NULL){
                (*bs)(objdesc);
                delete bs;
            }
            waiters.erase(objdesc->getObjectType());
        }
    }

}
