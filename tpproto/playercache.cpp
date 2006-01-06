/*  PlayerCache - Cache of Objects class
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
#include "player.h"
#include "getplayer.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "playercache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    PlayerCache::PlayerCache() : Cache(){
    }

    /*! \brief Destructor.
    */
    PlayerCache::~PlayerCache(){
    }

    /*! \brief Gets a Player from the cache.
    \param playerid The id of the Player to get.
    \return The Player, or NULL.
    */
    Player*  PlayerCache::getPlayer(uint32_t playerid){
        if(playerid != 0xffffffff)
            return static_cast<Player*>(cache->getById(playerid));
        else
            return NULL;
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

}
