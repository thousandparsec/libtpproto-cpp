/*  BoardCache - Cache of Boards class
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
    \return The Board, or NULL.
    */
    Board*  BoardCache::getBoard(uint32_t bid){
        return static_cast<Board*>(cache->getById(bid));
    }

    /*! \brief Set an Board Id as invalid and mark to be refetched.
    \param bid The id of the Board to invalidate.
    */
    void BoardCache::invalidateBoard(uint32_t bid){
        cache->markInvalid(bid);
    }

    /*! \brief Gets a set of all Board Ids.
    \return A set of Board Ids.
    */
    std::set<uint32_t> BoardCache::getBoardIds(){
        return cache->getAllIds();
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

}
