#ifndef TPPROTO_DESIGNCACHE_H
#define TPPROTO_DESIGNCACHE_H
/*  DesignCache - Cache of Designs class
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
  \brief Declares the DesignCache class.
*/

#include <set>

#include "cache.h"

namespace TPProto{

    class Design;

    /*! \brief A Cache that caches Designs.
    
    */
    class DesignCache : public Cache{
    public:
    DesignCache();
    virtual ~DesignCache();

    Design* getDesign(uint32_t designid);
    bool addDesign(Design* design);
    bool modifyDesign(Design* design);
    bool removeDesign(uint32_t designid);
    void invalidateDesign(uint32_t designid);

    std::set<uint32_t> getDesignIds();

    virtual GetIdSequence* createGetIdSequenceFrame();
    virtual GetById* createGetByIdFrame();
    virtual uint32_t getIdFromFrame(Frame* frame);
    virtual uint64_t getModTimeFromFrame(Frame* frame);

    };

}

#endif