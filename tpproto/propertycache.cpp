/*  PropertyCache - Cache of Properties class
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
#include "property.h"
#include "getproperty.h"
#include "getpropertyidslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "propertycache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    PropertyCache::PropertyCache() : Cache(){
    }

    /*! \brief Destructor.
    */
    PropertyCache::~PropertyCache(){
    }

    /*! \brief Gets a Property from the cache.
    \param propid The id of the Property to get.
    \return The Property, or NULL.
    */
    Property*  PropertyCache::getProperty(uint32_t propid){
        return static_cast<Property*>(cache->getById(propid));
    }

    /*! \brief Gets a set of all Property Ids.
    \return A set of Property Ids.
    */
    std::set<uint32_t> PropertyCache::getPropertyIds(){
        return cache->getAllIds();
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

}
