#ifndef TPPROTO_PLAYER_H
#define TPPROTO_PLAYER_H
/*  Player class
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

/*! \file
  \brief Declares the Player Frame.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

    /*! \brief A Frame of a Player from the server.
    */
    class Player : public Frame{
    public:
        Player();
        virtual ~Player();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        uint32_t getPlayerId() const;
        std::string getPlayerName() const;
        std::string getRaceName() const;

    protected:
        /*! \brief The Player's Id number.
        */
        uint32_t playerid;
        /*! \brief The Player's name.
        */
        std::string name;
        /*! \brief The Player's race.
        */
        std::string race;

    };

}

#endif
