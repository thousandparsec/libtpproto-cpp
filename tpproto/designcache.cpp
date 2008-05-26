/*  DesignCache - Cache of Designs class
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
#include "design.h"
#include "getdesign.h"
#include "getdesignidslist.h"
#include "protocollayer.h"
#include "framefactory.h"
#include "framecodec.h"
#include "adddesign.h"
#include "modifydesign.h"
#include "removedesign.h"

#include "designcache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    DesignCache::DesignCache() : Cache(), watchers(), waiters(){
    }

    /*! \brief Destructor.
    */
    DesignCache::~DesignCache(){
    }

    void DesignCache::requestDesign(uint32_t designid, const DesignCallback &cb){
        DesignSignal* bs = waiters[designid];
        if(bs == NULL){
            bs = new DesignSignal();
            waiters[designid] = bs;
        }
        bs->connect(cb);
        cache->getById(designid);
    }
    
    boost::signals::connection DesignCache::watchDesign(uint32_t designid, const DesignCallback &cb){
        DesignSignal *bs = watchers[designid];
        if(bs == NULL){
            bs = new DesignSignal();
            watchers[designid] = bs;
        }
        boost::signals::connection conn = bs->connect(cb);
        requestDesign(designid, cb);
        return conn;
    }

    /*! \brief Adds a Design to the server.
    \param design The Design to add.
    \return True if added successfully, false otherwise.
    */
    bool DesignCache::addDesign(Design* design){
        AddDesign* frame = protocol->getFrameFactory()->createAddDesign();
        frame->copyFromDesign(design);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL){
            if(reply->getType() == ft02_OK){
                
                delete reply;
                cache->update();
                return true;
            }
            delete reply;
        }
        return false;
    }

    /*! \brief Modifies a Design on the server.
    \param design The Design to modify.
    \return True if modified successfully, false otherwise.
    */
    bool DesignCache::modifyDesign(Design* design){
        ModifyDesign* frame = protocol->getFrameFactory()->createModifyDesign();
        frame->copyFromDesign(design);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        if(reply != NULL){
            if(reply->getType() == ft02_OK){
                
                delete reply;
                cache->markInvalid(design->getDesignId());
                return true;
            }
            delete reply;
        }
        return false;
    }

    /*! \brief Removes a design from the server.
    
    Sends the RemoveDesign frame and receives the reply.
    \param designid The Design Id to remove.
    \return True if sucessful, false otherwise.
  */
    bool DesignCache::removeDesign(uint32_t designid){
        RemoveDesign* frame = protocol->getFrameFactory()->createRemoveDesign();
        frame->removeDesignId(designid);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
        delete frame;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }
        
        if(reply != NULL && reply->getType() == ft02_OK){
            delete reply;
            cache->markInvalid(designid);
            return true;
            
        }
        delete reply;
        
        return false;
    }

    /*! \brief Set an Design Id as invalid and mark to be refetched.
    \param designid The id of the Design to invalidate.
    */
    void DesignCache::invalidateDesign(uint32_t designid){
        cache->markInvalid(designid);
    }

    void DesignCache::requestDesignIds(const IdSetCallback& cb){
        cache->getAllIds(cb);
    }
    
    boost::signals::connection DesignCache::watchDesignIds(const IdSetCallback& cb){
        return cache->watchAllIds(cb);
    }

    GetIdSequence* DesignCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetDesignIdsList();
    }

    GetById* DesignCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetDesign();
    }

    uint32_t DesignCache::getIdFromFrame(Frame* frame){
        Design* design = dynamic_cast<Design*>(frame);
        if(design != NULL && design->getType() == ft03_Design){
            return design->getDesignId();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t DesignCache::getModTimeFromFrame(Frame* frame){
        Design* design = dynamic_cast<Design*>(frame);
        if(design != NULL && design->getType() == ft03_Design){
            return design->getModTime();
        }else{
            return 0LL;
        }
    }

    void DesignCache::newItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Design> design(boost::dynamic_pointer_cast<Design>(item));
        if(design){
            DesignSignal* bs = waiters[design->getDesignId()];
            if(bs != NULL){
                (*bs)(design);
                delete bs;
            }
            waiters.erase(design->getDesignId());
            bs = watchers[design->getDesignId()];
            if(bs != NULL){
                (*bs)(design);
            }
        }
    }
    
    void DesignCache::existingItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Design> design(boost::dynamic_pointer_cast<Design>(item));
        if(design){
            DesignSignal* bs = waiters[design->getDesignId()];
            if(bs != NULL){
                (*bs)(design);
                delete bs;
            }
            waiters.erase(design->getDesignId());
        }
    }
    
}
