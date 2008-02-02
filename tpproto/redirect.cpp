/*  Redirect frame class
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

#include <cassert>

#include "buffer.h"

#include "redirect.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    Redirect::Redirect() : url(){
    }

    /*! \brief Required virtual destructor.
    */
    Redirect::~Redirect(){

    }

    /*! \brief Pack into a Buffer, Assert(0).
        
    This Frame does not get sent to the server, so this method asserts zero.
    \param buf Buffer to pack into, ignored.
    */ 
    void Redirect::packBuffer(Buffer *buf){
        // should never send a redirect frame
        assert(0);
    }

    /*! \brief Unpack from a Buffer.
        \param buf The Buffer to unpack out of.
        \return True if successful, false otherwise.
    */
    bool Redirect::unpackBuffer(Buffer *buf){
        url = buf->unpackString();
        
        type = ft03_Redirect;

        return true;
    }

    /*! \brief Gets the redirection url string sent in the Redirect frame.
        \return The url string.
    */
    std::string Redirect::getUrl(){
        return url;
    }

}
