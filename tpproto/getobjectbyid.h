#ifndef TPPROTO_GETOBJECTBYID_H
#define TPPROTO_GETOBJECTBYID_H
/*  GetObjectById class
 *
 *  Copyright (C) 2004-2006  Lee Begg and the Thousand Parsec Project
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
  \brief Declares GetObjectById Frame class.
*/

#include <tpproto/getbyid.h>
#include <set>

namespace TPProto{
  
  /*! \brief Frame to send to the server to get one or more Object by objectid.
  */
    class GetObjectById : public GetById{
    public:
        GetObjectById();
        virtual ~GetObjectById();

  };

}

#endif
