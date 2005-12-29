#ifndef TPPROTO_GETIDSEQUENCE_H
#define TPPROTO_GETIDSEQUENCE_H
/*  GetIdSequence baseclass
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
  \brief Declares the GetIdSequence BaseFrame.
*/

#include <stdint.h>

#include <tpproto/frame.h>

namespace TPProto{

    /*! \brief The GetIdSequence baseframe is used to get long lists of Ids from the server.
    */
    class GetIdSequence : public Frame{
    public:
        GetIdSequence();
        //GetIdSequence(GetIdSequence* ref);
        virtual ~GetIdSequence();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        void setSeqKey(uint32_t key);
        void setOffset(uint32_t os);
        void setCount(uint32_t c);

        uint32_t getSeqKey() const;
        uint32_t getOffset() const;
        uint32_t getCount() const;

    protected:
        /*! \brief The sequence key of the sequence to continue.
        Set to 0 if not know and to get a new one.
        */
        uint32_t seqkey;
        /*! \brief The offset in the sequence to get from.
        */
        uint32_t offset;
        /*! \brief The number of ids to get.
        */
        uint32_t count;

    };

}

#endif
