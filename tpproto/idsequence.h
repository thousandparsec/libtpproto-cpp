#ifndef TPPROTO_IDSEQUENCE_H
#define TPPROTO_IDSEQUENCE_H
/*  IdSequence baseframe class
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
  \brief Declares the IdSequence BaseFrame.
*/

#include <map>
#include <tpproto/frame.h>

namespace TPProto{

    /*! \brief The IdSequence baseframe is the basis for receiving long lists of ids.
    */
    class IdSequence : public Frame{
    public:
        IdSequence();
        virtual ~IdSequence();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        uint32_t getSeqKey() const;
        uint32_t getRemaining() const;
        std::map<uint32_t, uint64_t> getIds() const;

    protected:
        /*! \brief The sequence key for this sequence.
        */
        uint32_t seqkey;
        /*! \brief The number of items remaining in the sequence to be fetched.
        */
        uint32_t remaining;
        /*! \brief The map of ids and modification times.
        */
        std::map<uint32_t, uint64_t> idlist;

    };

}

#endif
