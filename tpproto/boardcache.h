#ifndef TPPROTO_BOARDCACHE_H
#define TPPROTO_BOARDCACHE_H
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

/*! \file
  \brief Declares the BoardCache class.
*/

#include <set>
#include <boost/signal.hpp>

#include "cache.h"

namespace TPProto{
    
    typedef boost::signal<void (boost::shared_ptr<Board>)> BoardSignal;
    typedef BoardSignal::slot_type BoardCallback;

    class Board;

    /*! \brief A Cache that caches Boards.
    
    */
    class BoardCache : public Cache{
    public:
    BoardCache();
    virtual ~BoardCache();

    void requestBoard(uint32_t bid, const BoardCallback &cb);
    boost::signals::connection watchBoard(uint32_t bid, const BoardCallback &cb);
    void invalidateBoard(uint32_t bid);
    
    void requestBoardIds(const IdSetCallback& cb);
    boost::signals::connection watchBoardIds(const IdSetCallback& cb);

    //Upcalls by cachemethod
    virtual GetIdSequence* createGetIdSequenceFrame();
    virtual GetById* createGetByIdFrame();
    virtual uint32_t getIdFromFrame(Frame* frame);
    virtual uint64_t getModTimeFromFrame(Frame* frame);
    
    virtual void newItem(boost::shared_ptr<Frame> item);
    virtual void existingItem(boost::shared_ptr<Frame> item);

        private:
            std::map<uint32_t, BoardSignal*> watchers;
            std::map<uint32_t, BoardSignal*> waiters;
    };

}

#endif
