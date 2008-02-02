/*  Board - Frame send by the server to tell the client about a board.
 *
 *  Copyright (C) 2005, 2006, 2008  Lee Begg and the Thousand Parsec Project
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

#include "board.h"

namespace TPProto{

  /*! \brief Default Constructor.
   */
  Board::Board(){
  }

  /*! \brief Virtual destructor as required.
   */
  Board::~Board(){
  }

  /*! \brief Packs the board into a buffer.

      Currently assets(0).  Should not be used, sending boards to the 
      server is not needed at this time.
      \param buf The buffer to pack into.
  */
  void Board::packBuffer(Buffer *buf){
    assert(0);
  }

  /*! \brief Unpacks a board from a buffer.

    Unpacks a buffer, setting the values given by the server.
    \param buf The buffer to unpack from.
    \return True if successful.
  */
  bool Board::unpackBuffer(Buffer* buf){
    id = buf->unpackInt();
    name = buf->unpackString();
    desc = buf->unpackString();

    nummess = buf->unpackInt();
    if(protoVer >= 3){
        modtime = buf->unpackInt64();
    }

    type = ft02_Board;

    return true;
  }

  /*! \brief Gets the board's id number.
      \return The Board ID number.
  */
  unsigned int Board::getId(){
    return id;
  }

  /*! \brief Gets the board's name.
      \return The board's name.
  */
  std::string Board::getName(){
    return name;
  }

  /*! \brief Gets the board's description.
    \return The board's description.
  */
  std::string Board::getDescription(){
    return desc;
  }

  /*! \brief Get the number of messages on the board.

      The id of the messages are 0 to numMessages() - 1.
      \return The number of messages.
  */
  int Board::numMessages(){
    return nummess;
  }
  
    /*! \brief Gets the last time this board was modified.
    \return The timestamp of the last modification time.
    */
    uint64_t Board::getLastModifiedTime(){
        return modtime;
    }

}
