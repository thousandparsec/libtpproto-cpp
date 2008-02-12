/*  ObjectParameterGroup - Send by the server tell the client about the a group of
 *     object parameters.
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
#include "objectparameter.h"
//object parameters
// #include "spacecoord.h"
// #include "timeparameter.h"
// #include "objectparameter.h"
// #include "listparameter.h"
// #include "spacecoordrel.h"
// #include "rangeparameter.h"
// #include "stringparameter.h"

#include "objectparamgroup.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  ObjectParameterGroup::ObjectParameterGroup(){
  }

  /*! \brief Copy constructor.
  */
  ObjectParameterGroup::ObjectParameterGroup(const ObjectParameterGroup& rhs){
    name = rhs.name;
    desc = rhs.desc;
    for(std::list<ObjectParameter*>::const_iterator itcurr = rhs.params.begin(); itcurr != rhs.params.end(); ++itcurr){
      params.push_back((*itcurr)->clone());
    }
  }
  
  /*! \brief Destructor.

  Deletes the OrderParameters it contains.
  */
  ObjectParameterGroup::~ObjectParameterGroup(){
    for(std::list<ObjectParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      delete *itcurr;
    }
    params.clear();
  }

  /*! \brief Unpack this ObjectParameterGroup from a Buffer.
    \param buf The Buffer to unpack out of.
    \return True if successful.
  */
  bool ObjectParameterGroup::unpackBuffer(Buffer* buf){
    groupid = buf->unpackInt();
    name = buf->unpackString();
    desc = buf->unpackString();

    //unpack args
    int numparams = buf->unpackInt();

    for(int i = 0; i < numparams; i++){
      std::string pname = buf->unpackString();
      int ptype = buf->unpackInt();
      std::string pdesc = buf->unpackString();
      ObjectParameter* opm = NULL;
      switch(ptype){
//       case opT_Space_Coord_Abs:
// 	opm = new SpaceCoordinates();
// 	break;
//       case opT_Time:
// 	opm = new TimeParameter();
// 	break;
//       case opT_Object_ID:
// 	opm = new ObjectParameter();
// 	break;
//       case opT_List:
// 	opm = new ListParameter();
// 	break;

      default:
        break;
      }
      if(opm != NULL){
        opm->setName(pname);
        opm->setDescription(pdesc);
        opm->unpackDescBuffer(buf);
        params.push_back(opm);
      }else{
        // unknown parameter type
        //TODO throw exception?
        return false;
        
      }
    }
    
    return true;
  }

  /*! \brief Gets the group id for this ObjectParameterGroup.
    \return The group Id.
  */
  unsigned int ObjectParameterGroup::getGroupId(){
    return groupid;
  }

  /*! \brief Gets the name of this ObjectParameterGroup.
    /return The name of the object parameter group.
  */
  std::string ObjectParameterGroup::getName(){
    return name;
  }

  /*! \brief Gets the text description of this ObjectParameterGroup.
    /return The text description of the object parameter group.
  */
  std::string ObjectParameterGroup::getDescription(){
    return desc;
  }

  /*! \brief Gets a copy of the ObjectParameters in this order description.
    \return A copy of the list of ObjectParameters.
  */
  std::list<ObjectParameter*> ObjectParameterGroup::getParameters(){
    std::list<ObjectParameter*> np;
    for(std::list<ObjectParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      np.push_back((*itcurr)->clone());
    }
    return np;
  }

}

