#ifndef TPPROTO_PROPERTYCACHE_H
#define TPPROTO_PROPERTYCACHE_H
/*  PropertyCache - Cache of Properties class
 *
 *  Copyright (C) 2006  Lee Begg and the Thousand Parsec Project
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
  \brief Declares the PropertyCache class.
*/

#include <set>

#include "cache.h"

namespace TPProto{

    class Property;

    /*! \brief A Cache that caches Properties.
    
    */
    class PropertyCache : public Cache{
    public:
    PropertyCache();
    virtual ~PropertyCache();

    Property* getProperty(uint32_t catid);

    std::set<uint32_t> getPropertyIds();

    virtual GetIdSequence* createGetIdSequenceFrame();
    virtual GetById* createGetByIdFrame();
    virtual uint32_t getIdFromFrame(Frame* frame);
    virtual uint64_t getModTimeFromFrame(Frame* frame);

    };

}

#endif
