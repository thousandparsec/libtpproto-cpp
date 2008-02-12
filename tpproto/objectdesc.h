#ifndef TPPROTO_OBJECTDESC_H
#define TPPROTO_OBJECTDESC_H
/*  ObjectDescription - Frame send by the server tell the client about the connects of an
 *     object type.
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
  \brief Declares ObjectDescription Frame.
*/

#include <string>
#include <list>
#include <stdint.h>

#include <tpproto/frame.h>

namespace TPProto{

  class ObjectParameterGroup;

  /*! \brief A description of object as sent as a Frame.
   */
  class ObjectDescription : public Frame{
  public:
    ObjectDescription();
    virtual ~ObjectDescription();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);
    
    uint32_t getObjectType();
    std::string getName();
    std::string getDescription();
    std::list<ObjectParameterGroup*> getParameterGroups();
    uint64_t getLastModifiedTime();

  private:
    uint32_t otype;
    std::string name;
    std::string desc;
    std::list<ObjectParameterGroup*> groups;
    uint64_t modtime;

  };

}

#endif
