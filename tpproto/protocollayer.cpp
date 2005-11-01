/*  ProtocolLayer class
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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "framecodec.h"
#include "framefactory.h"
#include "framebuilder.h"

#include "protocollayer.h"

namespace TPProto {

    /*! \brief Constructs object and sets up defaults.

    Defaults are
        - FrameFactory base impl
        - FrameBuilder base impl
        - FrameCodec.
    */
    ProtocolLayer::ProtocolLayer(){
        codec = new FrameCodec();
        factory = new FrameFactory();
        builder = new FrameBuilder();
        codec->setProtocolLayer(this);
        factory->setProtocolLayer(this);
        builder->setProtocolLayer(this);
    }

    /*! \brief Destructor, deletes contained objects.
    */
    ProtocolLayer::~ProtocolLayer(){
        delete codec;
        delete factory;
        delete builder;
    }

    /*! \brief Sets the FrameCodec
    Deletes the previous FrameCodec if any. If NULL is
    passed in, the default FrameCodec is created.
    \param fc The FrameCodec, or NULL.
    */
    void ProtocolLayer::setFrameCodec(FrameCodec* fc){
        delete codec;
        codec = fc;
        if(codec == NULL)
            codec = new FrameCodec();
        codec->setProtocolLayer(this);
    }

    /*! \brief Sets the FrameFactory
    Deletes the previous FrameFactory if any. If NULL is
    passed in, the default FrameFactory is created.
    \param ff The FrameFactory, or NULL.
    */
    void ProtocolLayer::setFrameFactory(FrameFactory* ff){
        delete factory;
        factory = ff;
        if(factory == NULL)
            factory = new FrameFactory();
        factory->setProtocolLayer(this);
    }

    /*! \brief Sets the FrameBuilder
    Deletes the previous FrameBuilder if any. If NULL is
    passed in, the default FrameBuilder is created.
    \param fd The FrameBuilder, or NULL.
    */
    void ProtocolLayer::setFrameBuilder(FrameBuilder* fb){
        delete builder;
        builder = fb;
        if(builder == NULL)
            builder = new FrameBuilder();
        builder->setProtocolLayer(this);
    }

    /*! \brief Gets the FrameCodec.
    \return The FrameCodec.
    */
    FrameCodec* ProtocolLayer::getFrameCodec() const{
        return codec;
    }

    /*! \brief Gets the FrameFactory.
    \return The FrameFactory.
    */
    FrameFactory* ProtocolLayer::getFrameFactory() const{
        return factory;
    }

    /*! \brief Gets the FrameBuilder.
    \return The FrameBuilder.
    */
    FrameBuilder* ProtocolLayer::getFrameBuilder() const{
        return builder;
    }

}
