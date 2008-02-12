/*  ObjectParameter - Base class for ObjectParameters.
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

#include "buffer.h"

#include "objectparameter.h"

namespace TPProto{

  /*! \brief Default Constructor.
   */
  ObjectParameter::ObjectParameter(){
  }

  /*! \brief Copy Constructor.

  Copys the name and description.
  \param rhs Reference to the ObjectParameter to be copied.
  */
  ObjectParameter::ObjectParameter(const ObjectParameter& rhs){
    name = rhs.name;
    description = rhs.description;
  }

  /*! \brief Required virtual destructor.
   */
  ObjectParameter::~ObjectParameter(){
  }

  /*! \brief Gets the name of this ObjectParameter.
    \return The name of the ObjectParameter.
  */
  std::string ObjectParameter::getName(){
    return name;
  }

  /*! \brief Gets the description of this ObjectParameter.
    \return The description of the ObjectParameter.
  */
  std::string ObjectParameter::getDescription(){
    return description;
  }

  /*! \brief Set the name of this ObjectParameter.
    \param nn The name to be set.
  */
  void ObjectParameter::setName(const std::string &nn){
    name = nn;
  }

  /*! \brief Set the description of this ObjectParameter.
    \param nd The description to be set.
  */
  void ObjectParameter::setDescription(const std::string & nd){
    description = nd;
  }

}
