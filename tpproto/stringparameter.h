#ifndef TPPROTO_STRINGPARAMETER_H
#define TPPROTO_STRINGPARAMETER_H
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

/*! \file
  \brief Declares the StringParameter OrderParameter.
*/

#include <string>
#include <tpproto/orderparameter.h>

namespace TPProto{

  /*! \brief An OrderParameter that holds a string with a maximum length.
   */
  class StringParameter : public OrderParameter{
  public:
    StringParameter();
    StringParameter(const StringParameter& rhs);
    ~StringParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    unsigned int getMaxLength();
    std::string getString();

    void setString(const std::string& nval);
    
  private:
    unsigned int maxlength;
    std::string value;
  };

}

#endif
