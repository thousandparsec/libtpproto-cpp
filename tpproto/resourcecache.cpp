/*  ResourceCache - Cache of ResourceDescriptions class
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cachemethod.h"
#include "resourcedesc.h"
#include "getresourcedesc.h"
#include "getresourcetypeslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "resourcecache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    ResourceCache::ResourceCache() : Cache(){
    }

    /*! \brief Destructor.
    */
    ResourceCache::~ResourceCache(){
    }

    /*! \brief Gets an ResourceDescription from the cache.
    \param restype The type of the ResourceDescription to get.
    \return The ResourceDescription, or NULL.
    */
    ResourceDescription*  ResourceCache::getResourceDescription(uint32_t restype){
        return static_cast<ResourceDescription*>(cache->getById(restype));
    }

    /*! \brief Gets a set of all Resource Types.
    \return A set of Resource type numbers.
    */
    std::set<uint32_t> ResourceCache::getResourceTypes(){
        return cache->getAllIds();
    }

    GetIdSequence* ResourceCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetResourceTypesList();
    }

    GetById* ResourceCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetResourceDescription();
    }

    uint32_t ResourceCache::getIdFromFrame(Frame* frame){
        ResourceDescription* res = dynamic_cast<ResourceDescription*>(frame);
        if(res != NULL && res->getType() == ft02_ResDesc){
            return res->getResourceType();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t ResourceCache::getModTimeFromFrame(Frame* frame){
        ResourceDescription* res = dynamic_cast<ResourceDescription*>(frame);
        if(res != NULL && res->getType() == ft02_ResDesc){
            return res->getModTime();
        }else{
            return 0LL;
        }
    }

}
