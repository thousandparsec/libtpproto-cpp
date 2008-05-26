#ifndef TPPROTO_RESOURCECACHE_H
#define TPPROTO_RESOURCECACHE_H
/*  ResourceCache - Cache of ResourceDescriptions class
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

/*! \file
  \brief Declares the ResourceCache class.
*/

#include <map>
#include <boost/signal.hpp>

#include "cache.h"

namespace TPProto{

    class ResourceDescription;
    
    typedef boost::signal<void (boost::shared_ptr<ResourceDescription>)> ResourceDescSignal;
    typedef ResourceDescSignal::slot_type ResourceDescCallback;

    /*! \brief A Cache that caches ResourceDescriptions.
    
    */
    class ResourceCache : public Cache{
    public:
    ResourceCache();
    virtual ~ResourceCache();
    
    void requestResourceDescription(uint32_t restype, const ResourceDescCallback &cb);
    boost::signals::connection watchResourceDescription(uint32_t restype, const ResourceDescCallback &cb);
    
    void requestResourceTypes(const IdSetCallback& cb);
    boost::signals::connection watchResourceTypes(const IdSetCallback& cb);

    virtual GetIdSequence* createGetIdSequenceFrame();
    virtual GetById* createGetByIdFrame();
    virtual uint32_t getIdFromFrame(Frame* frame);
    virtual uint64_t getModTimeFromFrame(Frame* frame);

    virtual void newItem(boost::shared_ptr<Frame> item);
    virtual void existingItem(boost::shared_ptr<Frame> item);

        private:
            std::map<uint32_t, ResourceDescSignal*> watchers;
            std::map<uint32_t, ResourceDescSignal*> waiters;
    };

}

#endif
