/*  CacheMethod - Methods of caching Frames base class
 *
 *  Copyright (C) 2006, 2008, 2009  Lee Begg and the Thousand Parsec Project
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
    CacheMethod::CacheMethod() : cache(NULL), protocol(NULL), waiters(), watchers(){
    }

    /*! \brief Copy Constructor, sets all fields to NULL.
        Subclasses should call this, and initialise any fields it might have.
    */
    CacheMethod::CacheMethod(const CacheMethod& rhs) : cache(NULL), protocol(NULL), waiters(), watchers(){
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

    /*! \brief Gets all the ids
    \param cb The callback to send the set of ids to..
     */
    void CacheMethod::getAllIds(const IdSetCallback& cb){
        waiters.connect(cb);
        getIdList();
    }
    
    /*! \brief Add an IdSetCallback to watch for changes to IdSet.
    \param cb The IdSetCallback to call when the IdSet changes.
    \return Connection to the signal for the callback. Allows management of the Signal connection.
    */
    boost::signals::connection CacheMethod::watchAllIds(const IdSetCallback& cb){
        waiters.connect(cb);
        boost::signals::connection conn = watchers.connect(cb);
        getIdList();
        return conn;
    }
    
    /*! \brief Sends a new set of Ids from the cache.
    The list is sent to all the waiters and the watches.
    \param list The set of Ids.
    */
    void CacheMethod::newIdList(std::set<uint32_t> list){
        waiters(list);
        waiters.disconnect_all_slots();
        watchers(list);
    }
    
    /*! \brief Sends an existing list of Ids from the cache.
    The list is only sent to the waiters, as there are no changes the
    watchers would be interested it.
    \param list The set of Ids.
    */
    void CacheMethod::existingList(std::set<uint32_t> list){
        waiters(list);
        waiters.disconnect_all_slots();
    }
}

