#ifndef TPPROTO_PROTOCOLLAYER_H
#define TPPROTO_PROTOCOLLAYER_H
/*  ProtocolLayer, the central object of the lower layer of libtpproto-cpp
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
 \brief Declares ProtocolLayer class.

    The ProtocolLayer class centrally holds the pointers to the FrameCodec, FrameBuilder and FrameFactory.
*/

namespace TPProto{

    class FrameCodec;
    class FrameBuilder;
    class FrameFactory;

    /*! \brief Central lower layer class.

    The ProtocolLayer class centrally holds the pointers to the FrameCodec, FrameBuilder and FrameFactory.
    This class is then referenced by the three classes and provides access to them as necessary.
    */
    class ProtocolLayer{
    public:
        ProtocolLayer();
        ~ProtocolLayer();

        void setFrameCodec(FrameCodec* fc);
        void setFrameFactory(FrameFactory* ff);
        void setFrameBuilder(FrameBuilder* fb);

        FrameCodec* getFrameCodec() const;
        FrameFactory* getFrameFactory() const;
        FrameBuilder* getFrameBuilder() const;

    private:
        FrameCodec* codec;
        FrameFactory* factory;
        FrameBuilder* builder;

    };

}

#endif
