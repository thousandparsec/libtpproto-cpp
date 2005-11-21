#ifndef TPPROTO_GETPROPERTY_H
#define TPPROTO_GETPROPERTY_H
/*  GetProperty class
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
  \brief Declares the GetProperty Frame.
*/

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief A Frame to get Properties from the server.
   */
  class GetProperty : public Frame{
  public:
    GetProperty();
    virtual ~GetProperty();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);
    
    void addPropertyId(unsigned int id);
    void addPropertyIds(std::set<unsigned int> ids);

  private:
    std::set<unsigned int> idset;

  };

}

#endif