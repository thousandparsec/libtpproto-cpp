#ifndef TPPROTO_GETCOMMANDTYPESLIST_H
#define TPPROTO_GETCOMMANDTYPESLIST_H
/*  GetCommandTypesList Admin Frame
 *
 *  Copyright (C) 2008 Aaron Mavrinac and the Thousand Parsec Project
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
    \brief Declares the GetCommandTypesList Frame.
*/

#include <tpproto/getidsequence.h>

namespace TPProto{

    /*! \brief The GetCommandTypesList frame requests the command type list.
    */
    class GetCommandTypesList : public GetIdSequence{
      public:
        GetCommandTypesList();
        virtual ~GetCommandTypesList();

        void packBuffer(Buffer * buf);

    };

}

#endif
