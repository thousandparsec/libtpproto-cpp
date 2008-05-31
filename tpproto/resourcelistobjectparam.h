#ifndef TPPROTO_RESOURCELISTOBJECTPARAM_H
#define TPPROTO_RESOURCELISTOBJECTPARAM_H
/*  ResourceListObjectParameter - Resource List ObjectParameters.
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
    
    struct ResourceCount{
        uint32_t stored;
        uint32_t minable;
        uint32_t unavailable;
    };
    
    class ResourceListObjectParam : public ObjectParameter{
        public:
            ResourceListObjectParam();
            ResourceListObjectParam(const ResourceListObjectParam& rhs);
            virtual ~ResourceListObjectParam();
            
            void packBuffer(Buffer* buf);
            bool unpackBuffer(Buffer* buf);
            bool unpackDescBuffer(Buffer* buf);
            ObjectParameter* clone();
            void visit(ObjectParameterVisitor* opv);
            
            std::map<uint32_t, ResourceCount> getResourceList() const;
            
        private:
            std::map<uint32_t, ResourceCount> reslist;
    };
}

#endif
