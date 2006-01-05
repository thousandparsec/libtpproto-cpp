#ifndef TPPROTO_CACHENONEMETHOD_H
#define TPPROTO_CACHENONEMETHOD_H
/*  CacheNoneMethod - Method of caching Frames that doesn't do any caching class
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
  \brief Declares the CacheNoneMethod class.
*/

#include "tpproto/cachemethod.h"

namespace TPProto{

    /*! \brief CacheMethod that doesn't cache anything class.
    */
    class CacheNoneMethod : public CacheMethod{
    public:
    CacheNoneMethod();
    CacheNoneMethod(const CacheNoneMethod& rhs);
    virtual ~CacheNoneMethod();

    virtual void update();

    virtual Frame* getById(uint32_t id);
    virtual void markInvalid(uint32_t id);

    virtual std::set<uint32_t> getAllIds();

    virtual CacheMethod* clone();

    };

}

#endif

