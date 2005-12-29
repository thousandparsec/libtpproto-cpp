#ifndef TPPROTO_GETOBJECTIDSBYCONTAINER_H
#define TPPROTO_GETOBJECTIDSBYCONTAINER_H
/*  GetObjectIdsByContainer frame class
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
  \brief Declares the GetObjectIdsByContainer Frame.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{
  
    /*! \brief The GetObjectIdsByContainer is sent to the server to find the ids of the objects contained by the one given.
    */
    class GetObjectIdsByContainer : public Frame{
    public:
        GetObjectIdsByContainer();
        virtual ~GetObjectIdsByContainer();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        uint32_t getId() const;
        void setId(uint32_t ni);

    protected:
        /*! \brief The object to get the object ids of the objects that are contained by it.
        */
        uint32_t obid;

    };

}

#endif
