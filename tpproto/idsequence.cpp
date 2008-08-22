/*  IdSequence baseframe class
 *
 *  Copyright (C) 2005, 2008  Lee Begg and the Thousand Parsec Project
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

#include "idsequence.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    IdSequence::IdSequence() : Frame(), serial((uint64_t)-1){
        seqkey = 0xffffffff;
        remaining = 0;
    }

    /*! \brief Required virtual destructor.
    */
    IdSequence::~IdSequence(){
    }

    /*! \brief Pack into a Buffer, asserts.
    
    Packs this IdSequence frame into the buffer. Since the frame only
    comes from the server, this method assert(0).
    \param buf Buffer to pack into, ignored.
    */ 
    void IdSequence::packBuffer(Buffer *buf){
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the IdSequence frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool IdSequence::unpackBuffer(Buffer *buf){
        seqkey = buf->unpackInt();
        remaining = buf->unpackInt();
        uint32_t numids = buf->unpackInt();
        for(uint32_t i = 0; i < numids; i++){
            uint32_t id = buf->unpackInt();
            idlist[id] = buf->unpackInt64();
        }
        if(protoVer >= 4){
          serial = buf->unpackInt64();
        }
        return true;
    }

    /*! \brief Gets the Sequence Key.

            Gets the value of the sequence key in use.
        \return The value of the seqkey.
     */
    uint32_t IdSequence::getSeqKey() const{
        return seqkey;
    }

    /*! \brief Gets the number of remaining ids to get.

        Gets the value of the number of ids that remain in this sequence to get.
        \return The value of the number of ids remaining.
     */
    uint32_t IdSequence::getRemaining() const{
        return remaining;
    }

    /*! \brief Gets the ids and modified time map.

        Gets the map of ids and modified time values that are in this
        part of the sequence..
        \return The map of ids and modified time.
     */
    std::map<uint32_t, uint64_t> IdSequence::getIds() const{
        return idlist;
    }
    
    /*! \brief Gets the minimum serial number of the ids list.
    
      If -1, then only existing ids are in the list. If not, then ids
      that have changed (including disappearing) since the serial (modtime) given
      are in the list.
      \return The minimum serial number.
    */
    uint64_t IdSequence::getSerialStart() const{
      return serial;
    }

}
