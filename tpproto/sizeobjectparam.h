#ifndef TPPROTO_SIZEOBJECTPARAM_H
#define TPPROTO_SIZEOBJECTPARAM_H
/*  SizeObjectParameter - Size ObjectParameters.
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
\brief Declears SizeObjectParam class.
*/

#include <tpproto/objectparameter.h>

namespace TPProto{
    /*! \brief ObjectParameter that holds the size of the object.
    */
    class SizeObjectParam : public ObjectParameter{
        public:
            SizeObjectParam();
            SizeObjectParam(const SizeObjectParam& rhs);
            virtual ~SizeObjectParam();
            
            void packBuffer(Buffer* buf);
            bool unpackBuffer(Buffer* buf);
            bool unpackDescBuffer(Buffer* buf);
            ObjectParameter* clone();
            void visit(ObjectParameterVisitor* opv);
            
            uint64_t getSize() const;
            
        private:
            uint64_t size;
    };
}

#endif
