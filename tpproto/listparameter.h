#ifndef TPPROTO_LISTPARAMETER_H
#define TPPROTO_LISTPARAMETER_H
/*  ListParameter - OrderParameter that presents a list of selectable things.
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
  \brief Declares the ListParameter class.
*/

#include <map>
#include <tpproto/orderparameter.h>

namespace TPProto{

  /*! \brief Name, max value pair for selection list.
   */
  typedef std::pair<std::string, unsigned int> SelectableElement;
  
  /*! \brief An OrderParameter containing a list of selectable items.
   */
  class ListParameter : public OrderParameter{
  public:
    ListParameter();
    ListParameter(const ListParameter& rhs);
    virtual ~ListParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    bool setSelection(unsigned int id, unsigned int num);
    unsigned int getSelection(unsigned int id);

    std::map<unsigned int, SelectableElement > getSelectableList();

  private:
    std::map<unsigned int, unsigned int> chosen;
    std::map<unsigned int, SelectableElement > selectable;
  };

}

#endif
