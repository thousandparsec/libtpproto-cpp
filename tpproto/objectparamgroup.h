#ifndef TPPROTO_OBJECTPARAMGROUP_H
#define TPPROTO_OBJECTPARAMGROUP_H
/*  ObjectParameterGroup - Sent by the server tell the client about a group of
 *      object Parameter.
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

/*! \file
  \brief Declares ObjectParameterGroup.
*/

#include <string>
#include <list>
#include <stdint.h>

namespace TPProto{

  class ObjectParameter;
  class Buffer;

  /*! \brief A description of object parameter group as sent in the ObjectDescription Frame.
   */
  class ObjectParameterGroup{
  public:
    ObjectParameterGroup();
    ObjectParameterGroup(const ObjectParameterGroup& rhs);
    ~ObjectParameterGroup();

    bool unpackBuffer(Buffer* buf);
    
    uint32_t getGroupId();
    std::string getName();
    std::string getDescription();
    std::list<ObjectParameter*> getParameters();

  private:
    uint32_t groupid;
    std::string name;
    std::string desc;
    std::list<ObjectParameter*> params;

  };

}

#endif
