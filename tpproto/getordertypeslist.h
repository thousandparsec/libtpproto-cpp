#ifndef TPPROTO_GETORDERTYPESLIST_H
#define TPPROTO_GETORDERTYPESLIST_H
/*  GetOrderTypesList frame class
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
  \brief Declares the GetOrderTypesList Frame.
*/

#include <tpproto/getidsequence.h>

namespace TPProto{

    /*! \brief The GetOrderTypesList frame requests the Order type list.
    */
    class GetOrderTypesList : public GetIdSequence{
    public:
        GetOrderTypesList();
        virtual ~GetOrderTypesList();

        void packBuffer(Buffer * buf);

    };

}

#endif
