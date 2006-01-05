#ifndef TPPROTO_CACHE_H
#define TPPROTO_CACHE_H
/*  Cache - Cache of Frames base class
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
  \brief Declares the Cache base class.
*/

#include <stdint.h>

namespace TPProto{

    class Frame;
    class GetIdSequence;
    class GetById;
    class CacheMethod;
    class ProtocolLayer;

    /*! \brief Interface to a cache.
    Most of the work is done by CacheMethod, so this class is mostly just an interface, with subclasses
    working to cast things and get ids, etc from the frames.
    */
    class Cache{
    public:
    Cache();
    virtual ~Cache();

    void update();
    void setCacheMethod(CacheMethod* cm);
    void setProtocolLayer(ProtocolLayer* pl);

    /*! \brief Create a GetIdsSequence subclass frame for the CacheMethod to use.
    Must return the correct type of frame for this Cache.
    */
    virtual GetIdSequence* createGetIdSequenceFrame() = 0;

    /*! \brief Create a GetById subclass frame for the CacheMethod to use.
    Must return the correct type of frame for this Cache.
    */
    virtual GetById* createGetByIdFrame() = 0;

    /*! \brief Get the Id from the Frame provided.
    The Frame is of the correct type. For example, for the Object cache, the Frame will be an Object.
    \param frame The Frame to get the Id from.
    \return The id of the frame.
    */
    virtual uint32_t getIdFromFrame(Frame* frame) = 0;

    /*! \brief Get the Modification time from the Frame provided.
    The Frame is of the correct type. For example, for the Object cache, the Frame will be an Object.
    \param frame The Frame to get the Modtime from.
    \return The modtime of the frame.
     */
    virtual uint64_t getModTimeFromFrame(Frame* frame) = 0;

    protected:
    /*! \brief The CacheMethod this Cache will use.
    */
    CacheMethod* cache;

    /*! \brief The ProtocolLayer that will be used.
    */
    ProtocolLayer* protocol;
    };

}

#endif

