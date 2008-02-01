#ifndef TPPROTO_SETFILTERS_H
#define TPPROTO_SETFILTERS_H
/*  SetFilters - Frame to tell the server what filters are wanted
 *
 *  Copyright (C) 2008  Lee Begg and the Thousand Parsec Project
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
  \brief Declares SetFilters Frame.
*/

#include <stdint.h>
#include <set>

#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief A Frame for tell the server what filters are wanted.
  This is made complex because of having to avoid pipelining, and setting
  state within the FrameCodec and Buffer objects. It probably isn't a good 
  idea for a client to use this class directly.
   */
  class SetFilters : public Frame{
  public:
    SetFilters();
    virtual ~SetFilters();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void addFilter(uint32_t fid);
    void removeFilter(uint32_t fid);
    std::set<uint32_t> getFilters() const;
    bool isSet(uint32_t fid) const;

  private:
    std::set<uint32_t> filters;
    
  };

}

#endif
