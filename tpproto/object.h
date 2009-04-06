#ifndef TPPROTO_OBJECT_H
#define TPPROTO_OBJECT_H
/*  Object - Frame send by the server to tell the client about an object.
 *
 *  Copyright (C) 2005, 2006, 2008, 2009  Lee Begg and the Thousand Parsec Project
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
  \brief Declares Object Frame baseclass.
*/

#include <stdint.h>
#include <string>
#include <set>
#include <list>
#include <boost/shared_ptr.hpp>

#include <tpproto/frame.h>

namespace TPProto{

  class ObjectDescription;
  class ObjectParameterGroup;

  /*! \brief The Object Frame class.
    
  All Objects in the game are instances of this class.
  Parameters from the ObjectDescription are used to give the
  type of this object.
  */
  class Object : public Frame{
  public:
      Object();
      ~Object();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    uint32_t getId();
    std::string getName();
    std::string getDescription();
    uint32_t getObjectType();
    uint32_t getParentId();
    std::set<uint32_t> getContainedObjectIds();
    uint64_t getLastModifiedTime();
    //something about parameters
    std::list<ObjectParameterGroup*> getParameters();
    
    void setObjectType(boost::shared_ptr<ObjectDescription> od);

  protected:
    /*! \brief The Object's Id
     */
    uint32_t id;
    

    /*! \brief The ObjectDescription for this object.
     */
    boost::shared_ptr<ObjectDescription> obtype;
    
    /*! \brief The object's name.
     */
    std::string name;
    
    /*! The object's description.
    */
    std::string description;
    
    /*! The object's parent objectid.
    */
    uint32_t parent;

    /*! \brief The set of objectids that this object contains.
     */
    std::set<uint32_t> contained;

    /*! \brief The last modification time of this object.
    */
    uint64_t modtime;
    
    /*! \brief The list of ObjectParameterGroup that contain the parameters.
    */
    std::list<ObjectParameterGroup*> paramgroups;

  };

}

#endif
