/*  Features baseframe class
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

#include <cassert>

#include "buffer.h"

#include "featuresframe.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    Features::Features() : Frame(){
    }

    /*! \brief Required virtual destructor.
    */
    Features::~Features(){
    }

    /*! \brief Pack into a Buffer, asserts.
    
    Packs this Features frame into the buffer. Since the frame only
    comes from the server, this method assert(0).
    \param buf Buffer to pack into, ignored.
    */ 
    void Features::packBuffer(Buffer *buf){
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        Unpacks the Features frame from the buffer.
    \param buf The Buffer to unpack out of.
    \return True if sucessful, false otherwise.
    */
    bool Features::unpackBuffer(Buffer *buf){
        uint32_t numfeat = buf->unpackInt();
        for(uint32_t i = 0; i < numfeat; i++){
            uint32_t id = buf->unpackInt();
            features.insert(id);
        }
        type = ft03_Features;
        return true;
    }

    /*! \brief Does this port support secure connection.
        \return True if the server supports secure connection on this port.
     */
    bool Features::hasSecureThisPort() const{
        return isSet(0x1);
    }

    /*! \brief Does another port support secure connection.
        \return True if the server supports secure connection on a different port.
    */
    bool Features::hasSecureOtherPort() const{
        return isSet(0x2);
    }

    /*! \brief Does this port support secure connection.
        \return True if the server supports secure connection on this port.
     */
    bool Features::hasHttpTunnelingThisPort() const{
        return isSet(0x3);
    }

    /*! \brief Does this port support secure connection.
        \return True if the server supports secure connection on this port.
     */
    bool Features::hasHttpTunnelingOtherPort() const{
        return isSet(0x4);
    }

    /*! \brief Does this port support secure connection.
        \return True if the server supports secure connection on this port.
     */
    bool Features::supportsPing() const{
        return isSet(0x5);
    }

    /*! \brief Does this port support secure connection.
        \return True if the server supports secure connection on this port.
     */
    bool Features::supportsServerSidePropertyCalc() const{
        return isSet(0x6);
    }

    /*! \brief Does this port support secure connection.
        \return True if the server supports secure connection on this port.
     */
    bool Features::isSet(uint32_t val) const{
        return (features.find(val) != features.end());
    }

}
