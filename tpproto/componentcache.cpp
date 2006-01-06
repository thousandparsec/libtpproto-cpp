/*  ComponentCache - Cache of Components class
 *
 *  Copyright (C) 2006  Lee Begg and the Thousand Parsec Project
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
    ComponentCache::ComponentCache() : Cache(){
    }

    /*! \brief Destructor.
    */
    ComponentCache::~ComponentCache(){
    }

    /*! \brief Gets an Component from the cache.
    \param compid The id of the Component to get.
    \return The Component, or NULL.
    */
    Component*  ComponentCache::getComponent(uint32_t compid){
        return static_cast<Component*>(cache->getById(compid));
    }

    /*! \brief Gets a set of all Component Ids.
    \return A set of Component Ids.
    */
    std::set<uint32_t> ComponentCache::getComponentIds(){
        return cache->getAllIds();
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

}
