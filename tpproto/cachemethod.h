#ifndef TPPROTO_CACHEMETHOD_H
#define TPPROTO_CACHEMETHOD_H
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

/*! \file
  \brief Declares the CacheMethod base class.
*/

#include <map>
#include <set>
#include <boost/signal.hpp>

namespace TPProto{

    class Frame;
    class Cache;
    class ProtocolLayer;
    
    typedef boost::signal<void (std::set<uint32_t>)> IdSetSignal;
    typedef IdSetSignal::slot_type IdSetCallback;

    /*! \brief Base class for a method of caching frames.
    Works in partnership with Cache to cache frames. CacheMethod determains how to cache the
    frames.
    */
    class CacheMethod{
    public:
    CacheMethod();
    CacheMethod(const CacheMethod& rhs);
    virtual ~CacheMethod();

    /*! \brief Called when the client wants to update the cache.
        Can do nothing.
    */
    virtual void update() = 0;

    void setCache(Cache* c);

    void setProtocolLayer(ProtocolLayer* pl);

    /*! \brief Gets a item from the cache by Id.
    
    Returned by call to Cache::newItem or Cache::existingItem
    
    \param id The Id of the item to get.
    */
    virtual void getById(uint32_t id) = 0;

    /*! \brief Marks an item as invalid.
    If a method wishes to it can refetch it from the server. The pointer to the
    frame should not be consided valid, and does not get deleted by the CacheMethod.
    \param id The id of the item that is invalid.
    */
    virtual void markInvalid(uint32_t id) = 0;

    /*! \brief Gets all the ids
    \param cb The callback to send the set of ids to..
    */
    void getAllIds(const IdSetCallback& cb);
    
    boost::signals::connection watchAllIds(const IdSetCallback& cb);

    /*! \brief Returns an empty clone of this CacheMethod.
    body should be return new CacheMethodNameCopyConstructor(this);
    */
    virtual CacheMethod* clone() = 0;

    protected:
        virtual void getIdList() = 0;
        void newIdList(std::set<uint32_t> list);
        void existingList(std::set<uint32_t> list);
        
    /*! \brief The Cache this CacheMethod will use.
    */
    Cache* cache;

    /*! \brief The ProtocolLayer that will be used.
    */
    ProtocolLayer* protocol;
    
    IdSetSignal waiters;
    IdSetSignal watchers;

    };

}

#endif

