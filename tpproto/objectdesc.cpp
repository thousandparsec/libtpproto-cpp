/*  ObjectDescription - Frame send by the server tell the client about the connects of an
 *     order type.
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

#include <cassert>

#include "buffer.h"
#include "objectparamgroup.h"

#include "objectdesc.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  ObjectDescription::ObjectDescription(){
  }

  /*! \brief Destructor.

  Deletes the OrderParameters it contains.
  */
  ObjectDescription::~ObjectDescription(){
    for(std::list<ObjectParameterGroup*>::iterator itcurr = groups.begin(); itcurr != groups.end(); ++itcurr){
      delete *itcurr;
    }
    groups.clear();
  }

  /*! \brief Pack into a Buffer, assert(0).

  This Frame is only received from the server and therefore does not get
  packed on the client side.  assert(0) is called.
    \param buf The Buffer, ignored.
  */
  void ObjectDescription::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpack this ObjectDescription from a Buffer.
    \param buf The Buffer to unpack out of.
    \return True if successful.
  */
  bool ObjectDescription::unpackBuffer(Buffer* buf){
    otype = buf->unpackInt();
    name = buf->unpackString();
    desc = buf->unpackString();
    modtime = buf->unpackInt64();
    //unpack args
    int numparams = buf->unpackInt();

    for(int i = 0; i < numparams; i++){
      ObjectParameterGroup *g = new ObjectParameterGroup();
      g->unpackBuffer(buf);
      groups.push_back(g);
    }

    type = ft04_ObjectDesc;

    return true;
  }

  /*! \brief Gets the object type number of this object description.
    \return The object type number.
  */
  unsigned int ObjectDescription::getObjectType(){
    return otype;
  }

  /*! \brief Gets the name of this object description.
    /return The name of the object description.
  */
  std::string ObjectDescription::getName(){
    return name;
  }

  /*! \brief Gets the text description of this object description.
    /return The text description of the object description.
  */
  std::string ObjectDescription::getDescription(){
    return desc;
  }

  /*! \brief Gets a copy of the ObjectParameterGroups in this object description.
    \return A copy of the list of ObjectParameterGroups.
  */
  std::list<ObjectParameterGroup*> ObjectDescription::getParameterGroups(){
    std::list<ObjectParameterGroup*> np;
    for(std::list<ObjectParameterGroup*>::iterator itcurr = groups.begin(); itcurr != groups.end(); ++itcurr){
      np.push_back(new ObjectParameterGroup(**itcurr));
    }
    return np;
  }

    /*! \brief Returns the timestamp of the last time this object description was modified.
    \return The timestamp of the modification time.
    */
    uint64_t ObjectDescription::getLastModifiedTime(){
        return modtime;
    }

}

