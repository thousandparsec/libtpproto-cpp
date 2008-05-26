/*  ComponentCache - Cache of Components class
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
#include "component.h"
#include "getcomponent.h"
#include "getcomponentidslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "componentcache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    ComponentCache::ComponentCache() : Cache(), watchers(), waiters(){
    }

    /*! \brief Destructor.
    */
    ComponentCache::~ComponentCache(){
    }

    /*! \brief Gets an Component from the cache.
    \param compid The id of the Component to get.
    \param cb The ComponentCallback to call to return the Component.
    */
    void ComponentCache::requestComponent(uint32_t compid, const ComponentCallback &cb){
        ComponentSignal* bs = waiters[compid];
        if(bs == NULL){
            bs = new ComponentSignal();
            waiters[compid] = bs;
        }
        bs->connect(cb);
        cache->getById(compid);
    }
    
    boost::signals::connection ComponentCache::watchComponent(uint32_t compid, const ComponentCallback &cb){
        ComponentSignal *bs = watchers[compid];
        if(bs == NULL){
            bs = new ComponentSignal();
            watchers[compid] = bs;
        }
        boost::signals::connection conn = bs->connect(cb);
        requestComponent(compid, cb);
        return conn;
    }

    void ComponentCache::requestComponentIds(const IdSetCallback& cb){
        cache->getAllIds(cb);
    }
    
    boost::signals::connection ComponentCache::watchComponentIds(const IdSetCallback& cb){
        return cache->watchAllIds(cb);
    }

    GetIdSequence* ComponentCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetComponentIdsList();
    }

    GetById* ComponentCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetComponent();
    }

    uint32_t ComponentCache::getIdFromFrame(Frame* frame){
        Component* comp = dynamic_cast<Component*>(frame);
        if(comp != NULL && comp->getType() == ft03_Component){
            return comp->getComponentId();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t ComponentCache::getModTimeFromFrame(Frame* frame){
        Component* comp = dynamic_cast<Component*>(frame);
        if(comp != NULL && comp->getType() == ft03_Component){
            return comp->getModTime();
        }else{
            return 0LL;
        }
    }

    void ComponentCache::newItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Component> component(boost::dynamic_pointer_cast<Component>(item));
        if(component){
            ComponentSignal* bs = waiters[component->getComponentId()];
            if(bs != NULL){
                (*bs)(component);
                delete bs;
            }
            waiters.erase(component->getComponentId());
            bs = watchers[component->getComponentId()];
            if(bs != NULL){
                (*bs)(component);
            }
        }
    }
    
    void ComponentCache::existingItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Component> component(boost::dynamic_pointer_cast<Component>(item));
        if(component){
            ComponentSignal* bs = waiters[component->getComponentId()];
            if(bs != NULL){
                (*bs)(component);
                delete bs;
            }
            waiters.erase(component->getComponentId());
        }
    }
    
}
