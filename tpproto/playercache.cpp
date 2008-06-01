/*  PlayerCache - Cache of Objects class
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

#include <boost/bind.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cachemethod.h"
#include "player.h"
#include "getplayer.h"
#include "protocollayer.h"
#include "framefactory.h"
#include "framecodec.h"

#include "playercache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    PlayerCache::PlayerCache() : Cache(), watchers(), waiters(), myplayerid(-1), myplayerwaiters(){
    }

    /*! \brief Destructor.
    */
    PlayerCache::~PlayerCache(){
    }

    void PlayerCache::update(){
        if(myplayerid == -1){
            GetPlayer* gp = protocol->getFrameFactory()->createGetPlayer();
            gp->addId(0);
            protocol->getFrameCodec()->sendFrame(boost::shared_ptr<GetPlayer>(gp), boost::bind(&PlayerCache::receiveMyPlayer, this, _1));
        }
        Cache::update();
    }
    
    void PlayerCache::requestPlayer(uint32_t pid, const PlayerCallback &cb){
        if(pid != 0xffffffff){
            if(pid == 0){
                if(myplayerid == -1){
                    myplayerwaiters.connect(cb);
                    return;
                }
                pid = myplayerid;
            }
            PlayerSignal* bs = waiters[pid];
            if(bs == NULL){
                bs = new PlayerSignal();
                waiters[pid] = bs;
            }
            bs->connect(cb);
            cache->getById(pid);
        }else{
            (cb.get_slot_function())(boost::shared_ptr<Player>());
        }
    }
    
    boost::signals::connection PlayerCache::watchPlayer(uint32_t pid, const PlayerCallback &cb){
        if(pid != 0xfffffff){
            PlayerSignal *bs = watchers[pid];
            if(bs == NULL){
                bs = new PlayerSignal();
                watchers[pid] = bs;
            }
            boost::signals::connection conn = bs->connect(cb);
            requestPlayer(pid, cb);
            return conn;
        }else{
            requestPlayer(pid, cb);
            return boost::signals::connection();
        }
    }
    
    void PlayerCache::requestPlayerIds(const IdSetCallback& cb){
        cache->getAllIds(cb);
    }
    
    boost::signals::connection PlayerCache::watchPlayerIds(const IdSetCallback& cb){
        return cache->watchAllIds(cb);
    }
    
    GetIdSequence* PlayerCache::createGetIdSequenceFrame(){
        return NULL;
    }

    GetById* PlayerCache::createGetByIdFrame(){
        return protocol->getFrameFactory()->createGetPlayer();
    }

    uint32_t PlayerCache::getIdFromFrame(Frame* frame){
        Player* player = dynamic_cast<Player*>(frame);
        if(player != NULL && player->getType() == ft03_Player){
            return player->getPlayerId();
        }else{
            return 0xffffffff;
        }
    }

    uint64_t PlayerCache::getModTimeFromFrame(Frame* frame){
        return 0LL;
    }

    void PlayerCache::newItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Player> player(boost::dynamic_pointer_cast<Player>(item));
        if(player){
            PlayerSignal* bs = waiters[player->getPlayerId()];
            if(bs != NULL){
                (*bs)(player);
                delete bs;
            }
            waiters.erase(player->getPlayerId());
            bs = watchers[player->getPlayerId()];
            if(bs != NULL){
                (*bs)(player);
            }
        }
        
    }
    
    void PlayerCache::existingItem(boost::shared_ptr<Frame> item){
        boost::shared_ptr<Player> player(boost::dynamic_pointer_cast<Player>(item));
        if(player){
            PlayerSignal* bs = waiters[player->getPlayerId()];
            if(bs != NULL){
                (*bs)(player);
                delete bs;
            }
            waiters.erase(player->getPlayerId());
        }
    }
    
    void PlayerCache::receiveMyPlayer(Frame* frame){
        Player* player = dynamic_cast<Player*>(frame);
        if(player){
            myplayerid = player->getPlayerId();
            myplayerwaiters(boost::shared_ptr<Player>(player));
            myplayerwaiters.disconnect_all_slots();
        }else{
            delete frame;
        }
    }
    
}
