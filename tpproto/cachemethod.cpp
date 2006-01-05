/*  CacheMethod - Methods of caching Frames base class
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

#include "cache.h"
#include "protocollayer.h"

#include "cachemethod.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    CacheMethod::CacheMethod() : cache(NULL), protocol(NULL){
    }

    /*! \brief Copy Constructor, sets all fields to NULL.
        Subclasses should call this, and initialise any fields it might have.
    */
    CacheMethod::CacheMethod(const CacheMethod& rhs) : cache(NULL), protocol(NULL){
    }

    /*! \brief Destructor, virtual
    */
    CacheMethod::~CacheMethod(){
    }

    /*! \brief Sets the Cache object this CacheMethod is for.
    CacheMethods can (and need to) use the Cache to get useful information.
    \param c The Cache this cache method will be used for.
    */
    void CacheMethod::setCache(Cache* c){
        cache = c;
    }

    /*! \brief Sets the ProtocolLayer to use.
    \param pl The ProtocolLayer to use.
    */
    void CacheMethod::setProtocolLayer(ProtocolLayer* pl){
        protocol = pl;
    }

}

