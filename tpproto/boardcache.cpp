/*  BoardCache - Cache of Boards class
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cachemethod.h"
#include "board.h"
#include "getboard.h"
#include "getboardidslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "boardcache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    BoardCache::BoardCache() : Cache(){
    }

    /*! \brief Destructor.
    */
    BoardCache::~BoardCache(){
    }

    /*! \brief Gets an Board from the cache.
    \param bid The id of the Board to get.
    \param cb The callback to call when the Board is available.
    */
    void BoardCache::requestBoard(uint32_t bid, const BoardCallback &cb){
        BoardSignal* bs = waiters[bid];
        if(bs == NULL){
            bs = new BoardSignal();
            waiters[bid] = bs;
        }
        bs->connect(cb);
        cache->getById(bid);
    }
    
    /*! \brief Watch for changes to a Board in the cache.
    
    The first time the board is change (or arrives for the first time) it is
    possible that two copies of the same board are delievered.
    
    \param bid The id of the Board to watch.
    \param cb The callback to call when the Board changes.
    \return connection object to manage the subscription to this board.
     */
    boost::signals::connection BoardCache::watchBoard(uint32_t bid, const BoardCallback &cb){
        BoardSignal *bs = watchers[bid];
        if(bs == NULL){
            bs = new BoardSignal();
            watchers[bid] = bs;
        }
        boost::signals::connection conn = bs->connect(cb);
        requestBoard(bid, cb);
        return conn;
    }
    
    /*! \brief Set an Board Id as invalid and mark to be refetched.
    \param bid The id of the Board to invalidate.
    */
    void BoardCache::invalidateBoard(uint32_t bid){
        cache->markInvalid(bid);
    }

    /*! \brief Gets a set of all Board Ids.
    Returned via IdSetCallback.
    */
    void BoardCache::requestBoardIds(const IdSetCallback& cb){
        cache->getAllIds(cb);
    }
    
    boost::signals::connection BoardCache::watchBoardIds(const IdSetCallback& cb){
        return cache->watchAllIds(cb);
    }

    GetIdSequence* BoardCache::createGetIdSequenceFrame(){
        return protocol->getFrameFactory()->createGetBoardIdsList();
    }

    GetById* BoardCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetBoard();
    }

    uint32_t BoardCache::getIdFromFrame(Frame* frame){
        Board* board = dynamic_cast<Board*>(frame);
        if(board != NULL && board->getType() == ft02_Board){
            return board->getId();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t BoardCache::getModTimeFromFrame(Frame* frame){
        Board* board = dynamic_cast<Board*>(frame);
        if(board != NULL && board->getType() == ft02_Board){
            return board->getLastModifiedTime();
        }else{
            return 0LL;
        }
    }

    void BoardCache::newItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Board> board(boost::dynamic_pointer_cast<Board>(item));
        if(board){
            BoardSignal* bs = waiters[board->getId()];
            if(bs != NULL){
                (*bs)(board);
                delete bs;
            }
            waiters.erase(board->getId());
            bs = watchers[board->getId()];
            if(bs != NULL){
                (*bs)(board);
            }
        }
        
    }
    
    void BoardCache::existingItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Board> board(boost::dynamic_pointer_cast<Board>(item));
        if(board){
            BoardSignal* bs = waiters[board->getId()];
            if(bs != NULL){
                (*bs)(board);
                delete bs;
            }
            waiters.erase(board->getId());
        }
    }
    
}
