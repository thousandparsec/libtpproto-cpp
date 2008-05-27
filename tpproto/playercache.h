#ifndef TPPROTO_PLAYERCACHE_H
#define TPPROTO_PLAYERCACHE_H
/*  PlayerCache - Cache of Players class
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
  \brief Declares the PlayerCache class.
*/

#include <map>
#include <boost/signal.hpp>

#include "cache.h"

namespace TPProto{

    class Player;
    
    typedef boost::signal<void (boost::shared_ptr<Player>)> PlayerSignal;
    typedef PlayerSignal::slot_type PlayerCallback;

    /*! \brief A Cache that caches Players.
    
    */
    class PlayerCache : public Cache{
    public:
    PlayerCache();
    virtual ~PlayerCache();
    
    void update();

    void requestPlayer(uint32_t pid, const PlayerCallback &cb);
    boost::signals::connection watchPlayer(uint32_t pid, const PlayerCallback &cb);
    
    void requestPlayerIds(const IdSetCallback& cb);
    boost::signals::connection watchPlayerIds(const IdSetCallback& cb);

    virtual GetIdSequence* createGetIdSequenceFrame();
    virtual GetById* createGetByIdFrame();
    virtual uint32_t getIdFromFrame(Frame* frame);
    virtual uint64_t getModTimeFromFrame(Frame* frame);
    
    virtual void newItem(boost::shared_ptr<Frame> item);
    virtual void existingItem(boost::shared_ptr<Frame> item);

        private:
            void receiveMyPlayer(Frame* frame);
            std::map<uint32_t, PlayerSignal*> watchers;
            std::map<uint32_t, PlayerSignal*> waiters;
            uint32_t myplayerid;
            PlayerSignal myplayerwaiters;
    };

}

#endif

