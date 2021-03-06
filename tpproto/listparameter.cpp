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

#include "buffer.h"
#include "orderparametervisitor.h"

#include "listparameter.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  ListParameter::ListParameter(){
  }

  /*! \brief Copy constructor.

  Does not copy the chosen items, but does copy the selectable list.
  \param rhs The ListParameter to copy.
  */
  ListParameter::ListParameter(const ListParameter& rhs) : OrderParameter(rhs){
    chosen.clear();
    copy(rhs.selectable.begin(), rhs.selectable.end(), inserter(selectable, selectable.begin()));
  }

  /*! \brief Destructor.
   */
  ListParameter::~ListParameter(){
    chosen.clear();
    selectable.clear();
  }

  /*! \brief Packs the ListParameter into a Buffer.

  The selectable list is not packed as the server does not need it (it sent
  it to us in the first place.)
  \param buf The Buffer to pack into.
  */
  void ListParameter::packBuffer(Buffer* buf){
    buf->packInt(0); // selectable list
    buf->packInt(chosen.size());
    for(std::map<unsigned int, unsigned int>::iterator itcurr = chosen.begin(); itcurr != chosen.end(); 
	++itcurr){
      buf->packInt(itcurr->first);
      buf->packInt(itcurr->second);
    }
  }

  /*! \brief Unpack the ListParameter from a Buffer.
    
  \param buf The Buffer to unpack from.
  \return True if unpacked successfully, false otherwise.
  */
  bool ListParameter::unpackBuffer(Buffer* buf){
    int numsa = buf->unpackInt();
    for(int i = 0; i < numsa; i++){
      unsigned int id = buf->unpackInt();
      std::string iname = buf->unpackString();
      unsigned int maxnum = buf->unpackInt();
      selectable[id] = SelectableElement(iname, maxnum);
    }
    int nums = buf->unpackInt();
    for(int i = 0; i < nums; i++){
      int id = buf->unpackInt();
      int num = buf->unpackInt();
      chosen[id] = num;
    }

    return true;
  }

  /*! \brief Clones this ListParameter.

  Just uses the copy constructor.
  \return A new copy of this ListParameter.
  */
  OrderParameter* ListParameter::clone(){
    return new ListParameter(*this);
  }

  /*! \brief Visits this OrderParameter.
    \param opv The OrderParameterVisitor that is trying to visit.
  */
  void ListParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Sets one item as selected with a given quanity.
    \param id The item id to select.
    \param num The number of the item to select.
    \return True if successful, false if number selected it too big or 
    the item is not found.
  */
  bool ListParameter::setSelection(unsigned int id, unsigned int num){
    std::map<unsigned int, SelectableElement>::iterator itcurr = 
    				selectable.find(id);
    if(itcurr != selectable.end() && num <= (itcurr->second.second)){
      chosen[id] = num;
      return true;
    }
    return false;
  }

  /*! \brief Gets the number selected for the given id.
    \param id The item id to check.
    \return The number of the item currently selected.
  */
  unsigned int ListParameter::getSelection(unsigned int id){
    return chosen[id];
  }

  /*! \brief Gets the map of SelectableElements.
    \return The map of item ids and SelectableElements.
  */
  std::map<unsigned int, SelectableElement> ListParameter::getSelectableList(){
    return selectable;
  }

}

