#ifndef TPPROTO_GETRESOURCEDESC_H
#define TPPROTO_GETRESOURCEDESC_H
/*  GetResourceDescription class
 *
 *  Copyright (C) 2005-2006  Lee Begg and the Thousand Parsec Project
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
  \brief Declares the GetResourceDescription Frame.
*/

#include <set>
#include <tpproto/getbyid.h>

namespace TPProto{

    /*! \brief A Frame to get Resource Descriptions from the server.
    */
    class GetResourceDescription : public GetById{
    public:
        GetResourceDescription();
        virtual ~GetResourceDescription();

    };

}

#endif
