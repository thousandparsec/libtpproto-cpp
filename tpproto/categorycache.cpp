/*  CategoryCache - Cache of Categories class
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
#include <boost/bind.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cachemethod.h"
#include "category.h"
#include "getcategory.h"
#include "getcategoryidslist.h"
#include "protocollayer.h"
#include "framefactory.h"
#include "framecodec.h"
#include "addcategory.h"
#include "removecategory.h"

#include "categorycache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    CategoryCache::CategoryCache() : Cache(), watchers(), waiters(){
    }

    /*! \brief Destructor.
    */
    CategoryCache::~CategoryCache(){
    }

   
    void CategoryCache::requestCategory(uint32_t catid, const CategoryCallback &cb){
        CategorySignal* bs = waiters[catid];
        if(bs == NULL){
            bs = new CategorySignal();
            waiters[catid] = bs;
        }
        bs->connect(cb);
        cache->getById(catid);
    }
    
    boost::signals::connection CategoryCache::watchCategory(uint32_t catid, const CategoryCallback &cb){
        CategorySignal *bs = watchers[catid];
        if(bs == NULL){
            bs = new CategorySignal();
            watchers[catid] = bs;
        }
        boost::signals::connection conn = bs->connect(cb);
        requestCategory(catid, cb);
        return conn;
    }
    
    /*! \brief Adds a Category to the server.
    \param cat The Category to add.
    */
    void CategoryCache::addCategory(Category* cat){
        AddCategory* frame = protocol->getFrameFactory()->createAddCategory();
        frame->setName(cat->getName());
        frame->setDescription(cat->getDescription());
        protocol->getFrameCodec()->sendFrame(boost::shared_ptr<Frame>(frame), boost::bind(&CategoryCache::receiveAddCategory, this, _1));
    }

    /*! \brief Removes a category from the server.
    
    Sends the RemoveCategory frame and receives the reply.
    \param catid The Category Id to remove.
  */
    void CategoryCache::removeCategory(uint32_t catid){
        RemoveCategory* frame = protocol->getFrameFactory()->createRemoveCategory();
        frame->removeCategoryId(catid);
        protocol->getFrameCodec()->sendFrame(boost::shared_ptr<Frame>(frame), boost::bind(&CategoryCache::receiveRemoveCategory, this, catid, _1));
    }

    /*! \brief Set an Category Id as invalid and mark to be refetched.
    \param catid The id of the Category to invalidate.
    */
    void CategoryCache::invalidateCategory(uint32_t catid){
        cache->markInvalid(catid);
    }

    void CategoryCache::requestCategoryIds(const IdSetCallback& cb){
        cache->getAllIds(cb);
    }
    
    boost::signals::connection CategoryCache::watchCategoryIds(const IdSetCallback& cb){
        return cache->watchAllIds(cb);
    }

    GetIdSequence* CategoryCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetCategoryIdsList();
    }

    GetById* CategoryCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetCategory();
    }

    uint32_t CategoryCache::getIdFromFrame(Frame* frame){
        Category* cat = dynamic_cast<Category*>(frame);
        if(cat != NULL && cat->getType() == ft03_Category){
            return cat->getCategoryId();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t CategoryCache::getModTimeFromFrame(Frame* frame){
        Category* cat = dynamic_cast<Category*>(frame);
        if(cat != NULL && cat->getType() == ft03_Category){
            return cat->getModTime();
        }else{
            return 0LL;
        }
    }

    void CategoryCache::newItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Category> category(boost::dynamic_pointer_cast<Category>(item));
        if(category){
            CategorySignal* bs = waiters[category->getCategoryId()];
            if(bs != NULL){
                (*bs)(category);
                delete bs;
            }
            waiters.erase(category->getCategoryId());
            bs = watchers[category->getCategoryId()];
            if(bs != NULL){
                (*bs)(category);
            }
        }
        
    }
    
    void CategoryCache::existingItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Category> category(boost::dynamic_pointer_cast<Category>(item));
        if(category){
            CategorySignal* bs = waiters[category->getCategoryId()];
            if(bs != NULL){
                (*bs)(category);
                delete bs;
            }
            waiters.erase(category->getCategoryId());
        }
    }
    
    void CategoryCache::receiveAddCategory(Frame* frame){
        if(frame->getType() == ft02_OK){
            update();
        }
    }
    
    void CategoryCache::receiveRemoveCategory(uint32_t catid, Frame* frame){
        if(frame->getType() == ft02_OK){
            invalidateCategory(catid);
            update();
        }
    }
    
}
