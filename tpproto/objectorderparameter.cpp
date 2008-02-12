/*  ObjectOrderDescription - Order parameter for referencing an Object
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

#include "buffer.h"
#include "orderparametervisitor.h"

#include "objectorderparameter.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  ObjectOrderParameter::ObjectOrderParameter(){
  }

  /*! \brief Copy constructor.
    \param rhs The ObjectOrderParameter to copy.
  */
  ObjectOrderParameter::ObjectOrderParameter(const ObjectOrderParameter& rhs) : OrderParameter(rhs){
    objectid = rhs.objectid;
  }

  /*! \brief Required virtual destructor.
   */
  ObjectOrderParameter::~ObjectOrderParameter(){
  }

  /*! \brief Pack into a Buffer.
    \param buf The Buffer to pack into.
  */
  void ObjectOrderParameter::packBuffer(Buffer* buf){
    buf->packInt(objectid);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool ObjectOrderParameter::unpackBuffer(Buffer* buf){
    objectid = buf->unpackInt();

    return true;
  }

  /*! \brief Clone this ObjectOrderParameter.
    \return A new copy of this ObjectOrderParameter.
  */
  OrderParameter* ObjectOrderParameter::clone(){
    return new ObjectOrderParameter(*this);
  }

  /*! \brief Visit this ObjectOrderParameter.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void ObjectOrderParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the Objectid this parameter currently points to.
    \return The objectid.
  */
  unsigned int ObjectOrderParameter::getObjectId(){
    return objectid;
  }

  /*! \brief Sets the object id this parameter points to.
    \param id The object id that his parameter should refer to.
  */
  void ObjectOrderParameter::setObjectId(unsigned int id){
    objectid = id;
  }

}
