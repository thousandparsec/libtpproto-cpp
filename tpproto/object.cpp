/*  Object - Frame send by the server to tell the client about an object.
 *
 *  Copyright (C) 2005, 2006, 2008  Lee Begg and the Thousand Parsec Project
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
#include "objectdesc.h"

#include "object.h"

namespace TPProto{

    /*! Constructor.
    */
    Object::Object() : Frame(), id(0), obtype(), name(), description(), parent(0), contained(), modtime(0){
    }
    
  /*! \brief Required destructor.
   */
  Object::~Object(){
  }

  /*! \brief Asserts(0).
    
  Thie method asserts zero because it should never be sent to the server.
  /param buf The Buffer (ignored).
  */
  void Object::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpacks the standard Object fields from the Buffer.

  Everything except the ObjectType specific data is unpacked.
  /param buf The Buffer the Object is to be unpacked from.
  \return True if Successful.
  */
  bool Object::unpackBuffer(Buffer* buf){
    id = buf->unpackInt();
    //skip the ob type because it has to already be set.
    buf->unpackInt();
    name = buf->unpackString();
    if(protoVer >= 4){
        description = buf->unpackString();
        parent = buf->unpackInt();
    }else{
        //size = buf->unpackInt64();
        //pos.unpack(buf);
        //vel.unpack(buf);
    }
     /* FIXME: Sanity check? */
    int count = buf->unpackInt();
    contained.clear();
    for(int i = 0; i < count; i++){
      contained.insert(buf->unpackInt());
    }
    if(protoVer < 4){
//         availableorders.clear();
//         /* FIXME: Sanity check? */
//         count = buf->unpackInt();
//         for(int i = 0; i < count; i++){
//         availableorders.insert(buf->unpackInt());
//         }
//         numorders = buf->unpackInt();
//         //4 unint32 padding (TP02), or modtime and 2 uint32 padding (TP03)
//         if(protoVer == 2){
//             buf->unpackInt();
//             buf->unpackInt();
//         }else{
//             modtime = buf->unpackInt64();
//         }
//         buf->unpackInt();
//         buf->unpackInt();
    }else{
        modtime = buf->unpackInt64();
        buf->unpackInt();
        buf->unpackInt();
        buf->unpackInt();
        buf->unpackInt();
    }

    //unpack parameters
    //TODO
    
    type = ft02_Object;

    return true;

  }

  /*! \brief Gets the Object's id number.
    \return The id number.
  */
  uint32_t Object::getId(){
    return id;
  }

  /*! \brief Gets the Object's name.
    \return The name of the object.
  */
  std::string Object::getName(){
    return name;
  }

  /*! \brief Gets the Object's description.
  \return The description of the object.
  */
  std::string Object::getDescription(){
      return description;
  }
  
  /*! \brief Gets the object type number of the object.
    \return The object type number.
  */
  uint32_t Object::getObjectType(){
    return obtype->getObjectType();
  }

  /*! \brief Gets the Parent object's Id.
  \return The Objectid of the Parent of this object.
  */
  uint32_t Object::getParentId(){
      return parent;
  }
  
  /*! \brief Gets the set of contained objectids.
    \return The set of contained object ids.
  */
  std::set<uint32_t> Object::getContainedObjectIds(){
    return contained;
  }

  /*! \brief Gets the last time this object was modified.
  \return The timestamp the object was last modified.
  */
  uint64_t Object::getLastModifiedTime(){
    return modtime;
  }
  
  /*! \brief Sets the ObjectDescription this Object should use.
  Must be set before calling unpackBuffer.
  \param od The ObjectDescription.
  */
  void Object::setObjectType(boost::shared_ptr<ObjectDescription> od){
      obtype = od;
      //parameters
      //TODO
  }

}

