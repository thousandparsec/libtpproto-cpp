#ifndef TPPROTO_REFQUANTLISTOBJECTPARAM_H
#define TPPROTO_REFQUANTLISTOBJECTPARAM_H
/*  RefQuantityListObjectParameter - Reference Quantity List ObjectParameters.
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

#include <map>
#include <tpproto/objectparameter.h>

namespace TPProto{
    
    class RefQuantityListObjectParam : public ObjectParameter{
        public:
            RefQuantityListObjectParam();
            RefQuantityListObjectParam(const RefQuantityListObjectParam& rhs);
            virtual ~RefQuantityListObjectParam();
            
            void packBuffer(Buffer* buf);
            bool unpackBuffer(Buffer* buf);
            bool unpackDescBuffer(Buffer* buf);
            ObjectParameter* clone();
            void visit(ObjectParameterVisitor* opv);
            
            typedef std::pair<int32_t, uint32_t> RefTypeAndId;
            typedef std::map<RefTypeAndId, uint32_t> RefQuantityList;
            
            RefQuantityList getRefQuantityList() const;
            
        private:
            RefQuantityList list;
    };
}

#endif
