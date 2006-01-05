/*  Cache class
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

#include "cachenonemethod.h"
#include "protocollayer.h"

#include "cache.h"

namespace TPProto {

    /*! \brief Constructor that sets up defaults.
    Default is for CacheNoneMethod CacheMethod.
    */
    Cache::Cache() : cache(new CacheNoneMethod()), protocol(NULL){
    }

    /*! \brief Destructor.
    */
    Cache::~Cache(){
        delete cache;
    }

    /*! \brief Call when you want to update the cache.
    Good times to do this are when we first log in, and when we notice an EOT has finished.
    */
    void Cache::update(){
        cache->update();
    }

    /*! \brief Sets the CacheMethod this Cache will use.
    \param cm The CacheMethod to use.
    */
    void Cache::setCacheMethod(CacheMethod* cm){
        if(cm == NULL){
            cm = new CacheNoneMethod();
        }
        cache = cm;
        cache->setCache(this);
        cache->setProtocolLayer(protocol);
    }

    /*! \brief Sets the ProtocolLayer this Cache will use.
    \param pl The ProtocolLayer to use.
    */
    void Cache::setProtocolLayer(ProtocolLayer* pl){
        protocol = pl;
        cache->setProtocolLayer(protocol);
    }

}
