#ifndef TPPROTO_POSITION3DOBJECTPARAM_H
#define TPPROTO_POSITION3DOBJECTPARAM_H
/*  Position3dObjectParameter - Position3d ObjectParameters.
 *
 *  Copyright (C) 2008  Lee Begg and the Thousand Parsec Project
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

#include <tpproto/objectparameter.h>
#include <tpproto/vector3d.h>

namespace TPProto{
    
    class Position3dObjectParam : public ObjectParameter{
        public:
            Position3dObjectParam();
            Position3dObjectParam(const Position3dObjectParam& rhs);
            virtual ~Position3dObjectParam();
            
            void packBuffer(Buffer* buf);
            bool unpackBuffer(Buffer* buf);
            bool unpackDescBuffer(Buffer* buf);
            ObjectParameter* clone();
            void visit(ObjectParameterVisitor* opv);
            
            Vector3d getPosition() const;
            uint32_t getRelativeObject() const;
            
        private:
            Vector3d pos;
            uint32_t relative;
    };
}

#endif
