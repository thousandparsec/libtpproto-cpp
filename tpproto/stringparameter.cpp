/*  StringParameter - OrderParameter that contains a string..
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

#include "stringparameter.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  StringParameter::StringParameter(){
  }

  /*! \brief Copy constructor.
    \param rhs The StringParameter to copy.
   */
  StringParameter::StringParameter(const StringParameter& rhs) : OrderParameter(rhs){
    maxlength = rhs.maxlength;
    value = rhs.value;
  }

  /*! /brief Required virtual destructor.
   */
  StringParameter::~StringParameter(){
  }

  /*! \brief Pack into a Buffer.

  The maxlength is packed as zero because it is a read only field.
    \param buf The Buffer to pack into.
  */
  void StringParameter::packBuffer(Buffer* buf){
    buf->packInt(0);
    buf->packString(value);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */ 
  bool StringParameter::unpackBuffer(Buffer* buf){
    maxlength = buf->unpackInt();
    value = buf->unpackString();

    return true;
  }

  /*! \brief Creates a copy of this StringParameter.
    \return A new copy of this StringParameter.
  */
  OrderParameter* StringParameter::clone(){
    return new StringParameter(*this);
  }

  /*! \brief Visit this StringParameter with an OrderParameterVisitor.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void StringParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the maximum length of the string in bytes.
    \return Maximum length of the string in bytes.
  */
  unsigned int StringParameter::getMaxLength(){
    return maxlength;
  }

  /*! \brief Gets the current string parameter.
    \returns The string value.
  */
  std::string StringParameter::getString(){
    return value;
  }

  /*! \brief Sets the string parameter.
    \param nval The string value to use.
  */
  void StringParameter::setString(const std::string& nval){
    value = nval;
  }

}
