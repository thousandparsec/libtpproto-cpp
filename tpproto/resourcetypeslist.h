#ifndef TPPROTO_RESOURCETYPESLIST_H
#define TPPROTO_RESOUCETYPESLIST_H
/*  ResourceTypesList frame class
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
  \brief Declares the ResourceTypesList Frame.
*/

#include <tpproto/idsequence.h>

namespace TPProto{

    /*! \brief The ResourceTypesList frame provides the list of Resource Types.
    */
    class ResourceTypesList : public IdSequence{
    public:
        ResourceTypesList();
        virtual ~ResourceTypesList();

        bool unpackBuffer(Buffer * buf);

    };

}

#endif
