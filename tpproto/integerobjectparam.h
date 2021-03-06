#ifndef TPPROTO_INTEGEROBJECTPARAM_H
#define TPPROTO_INTEGEROBJECTPARAM_H
/*  IntegerObjectParameter - Integer ObjectParameters.
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
\brief Declears the IntegerObjectParam class.
*/

#include <tpproto/objectparameter.h>

namespace TPProto{
    
    /*! \brief ObjectParameter to hold an Integer value.
    Used for example in Minisec's Universe Object to hold the turn number.
    */
    class IntegerObjectParam : public ObjectParameter{
        public:
            IntegerObjectParam();
            IntegerObjectParam(const IntegerObjectParam& rhs);
            virtual ~IntegerObjectParam();
            
            void packBuffer(Buffer* buf);
            bool unpackBuffer(Buffer* buf);
            bool unpackDescBuffer(Buffer* buf);
            ObjectParameter* clone();
            void visit(ObjectParameterVisitor* opv);
            
            uint32_t getValue() const;
            
        private:
            uint32_t value;
    };
}

#endif
