/*  Player frame class
 *
 *  Copyright (C) 2005  Lee Begg and the Thousand Parsec Project
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

#include <cassert>

#include "buffer.h"

#include "player.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    Player::Player() : Frame(), playerid(0), name(), race(){
    }

    /*! \brief Required virtual destructor.
    */
    Player::~Player(){
    }

    /*! \brief Pack into a Buffer, asserts.
    
    Packs this Player frame into the buffer. Since the frame only
    comes from the server, this method assert(0).
    \param buf Buffer to pack into, ignored.
    */ 
    void Player::packBuffer(Buffer *buf){
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the Player frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool Player::unpackBuffer(Buffer *buf){
        playerid = buf->unpackInt();
        name = buf->unpackString();
        race = buf->unpackString();
        type = ft03_Player;
        return true;
    }

    /*! \brief Gets the player id.
        \return The player's id number.
     */
    uint32_t Player::getPlayerId() const{
        return playerid;
    }

    /*! \brief Gets the player name.
        \return The player's name.
    */
    std::string Player::getPlayerName() const{
        return name;
    }

    /*! \brief Gets the name of the player's race.
        \return The name of the player's race.
    */
    std::string Player::getRaceName() const{
        return race;
    }
}
