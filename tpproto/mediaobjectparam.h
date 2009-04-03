#ifndef TPPROTO_MEDIAOBJECTPARAM_H
#define TPPROTO_MEDIAOBJECTPARAM_H
/*  MediaObjectParam - Media ObjectParameters.
 *
 *  Copyright (C) 2008, 2009  Lee Begg and the Thousand Parsec Project
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
 \brief Declears the MediaObjectParam class.
*/

#include <string>

#include <tpproto/objectparameter.h>

namespace TPProto{
    
    /*! \brief The MediaObjectParam is an ObjectParameter that references a peice of media.
    
    Using the base media URL in the GameFrame for the game,and the types the client supports,
    the media can be found and/or downloaded for the client to display.
    */
    class MediaObjectParam : public ObjectParameter{
        public:
            MediaObjectParam();
            MediaObjectParam(const MediaObjectParam& rhs);
            virtual ~MediaObjectParam();
            
            void packBuffer(Buffer* buf);
            bool unpackBuffer(Buffer* buf);
            bool unpackDescBuffer(Buffer* buf);
            ObjectParameter* clone();
            void visit(ObjectParameterVisitor* opv);
            
            std::string getMediaString() const;
            
        private:
            std::string media;
    };
}

#endif
