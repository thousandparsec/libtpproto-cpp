#ifndef TPPROTO_FRAMEBUILDER_H
#define TPPROTO_FRAMEBUILDER_H
/*  FrameBuilder - creates frame objects from type number
 *
 *  Copyright (C) 2005, 2008  Lee Begg and the Thousand Parsec Project
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
  \brief Declares the FrameBuilder class.
*/

#include <stdint.h>
#include <boost/shared_ptr.hpp>

namespace TPProto{

    class ProtocolLayer;
    class Frame;
    class Object;
    class Order;
    class Buffer;
    class OrderDescCache;
    class ObjectDescCache;

    /*! \brief Builds Frames from frame type and Buffer.

    Also creates  Objects from object type, and Orders from order types.
    */
    class FrameBuilder{
    public:
        FrameBuilder();
        virtual ~FrameBuilder();

        void setProtocolLayer(ProtocolLayer* pl);
        void setOrderDescCache(OrderDescCache* odc);
        void setObjectDescCache(ObjectDescCache* odc);
        
        virtual void buildFrame(uint32_t type, Buffer* data, uint32_t ftver, uint32_t seqnum);
        
    protected:
        void processOrderDescription(Frame* frame, Buffer* data, boost::shared_ptr<OrderDescription> od);
        void processObjectDescription(Frame* frame, Buffer* data, boost::shared_ptr<ObjectDescription> od);
        
        /*! \brief The ProtocolLayer this FrameBuilder is part of.
        */
        ProtocolLayer* layer;
        
        /*! The OrderDescCache to use to create Orders.
        */
        OrderDescCache* orderdesccache;

        /*! The ObjectDescCache to use to create Objects.
        */
        ObjectDescCache* objectdesccache;
        
    };

}

#endif
