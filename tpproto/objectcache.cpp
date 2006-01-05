/*  ObjectCache - Cache of Objects class
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
#include "object.h"
#include "getobjectbyid.h"
#include "getobjectidslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "objectcache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    ObjectCache::ObjectCache() : Cache(){
    }

    /*! \brief Destructor.
    */
    ObjectCache::~ObjectCache(){
    }

    /*! \brief Gets an Object from the cache.
    \param obid The id of the Object to get.
    \return The Object, or NULL.
    */
    Object*  ObjectCache::getObject(uint32_t obid){
        return static_cast<Object*>(cache->getById(obid));
    }

    /*! \brief Set an Object Id as invalid and mark to be refetched.
    \param obid The id of the Object to invalidate.
    */
    void ObjectCache::invalidateObject(uint32_t obid){
        cache->markInvalid(obid);
    }

    /*! \brief Gets a set of all Object Ids.
    \return A set of Object Ids.
    */
    std::set<uint32_t> ObjectCache::getObjectIds(){
        return cache->getAllIds();
    }

    GetIdSequence* ObjectCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetObjectIdsList();
    }

    GetById* ObjectCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetObjectById();
    }

    uint32_t ObjectCache::getIdFromFrame(Frame* frame){
        Object* object = dynamic_cast<Object*>(frame);
        if(object != NULL && object->getType() == ft02_Object){
            return object->getId();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t ObjectCache::getModTimeFromFrame(Frame* frame){
        Object* object = dynamic_cast<Object*>(frame);
        if(object != NULL && object->getType() == ft02_Object){
            return object->getLastModifiedTime();
        }else{
            return 0LL;
        }
    }

}
