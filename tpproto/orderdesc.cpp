/*  OrderDescription - Frame send by the server tell the client about the connects of an
 *     order type.
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
#include "orderparameter.h"
//order parameters
#include "spacecoord.h"
#include "timeparameter.h"
#include "objectorderparameter.h"
#include "listparameter.h"
#include "spacecoordrel.h"
#include "rangeparameter.h"
#include "stringparameter.h"

#include "orderdesc.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  OrderDescription::OrderDescription(){
  }

  /*! \brief Destructor.

  Deletes the OrderParameters it contains.
  */
  OrderDescription::~OrderDescription(){
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      delete *itcurr;
    }
    params.clear();
  }

  /*! \brief Pack into a Buffer, assert(0).

  This Frame is only received from the server and therefore does not get
  packed on the client side.  assert(0) is called.
    \param buf The Buffer, ignored.
  */
  void OrderDescription::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpack this OrderDescription from a Buffer.
    \param buf The Buffer to unpack out of.
    \return True if successful.
  */
  bool OrderDescription::unpackBuffer(Buffer* buf){
    otype = buf->unpackInt();
    name = buf->unpackString();
    desc = buf->unpackString();

    //unpack args
    int numparams = buf->unpackInt();

    for(int i = 0; i < numparams; i++){
      std::string temp = buf->unpackString();
      int ptype = buf->unpackInt();
      OrderParameter* opm = NULL;
      switch(ptype){
      case opT_Space_Coord_Abs:
	opm = new SpaceCoordinates();
	break;
      case opT_Time:
	opm = new TimeParameter();
	break;
      case opT_Object_ID:
	opm = new ObjectOrderParameter();
	break;
      case opT_List:
	opm = new ListParameter();
	break;

      default:
	break;
      }
      if(opm != NULL){
	opm->setName(temp);
      }

      temp = buf->unpackString();
      if(opm != NULL){
	opm->setDescription(temp);
	params.push_back(opm);
      }
    }
    if(protoVer >= 3){
        modtime = buf->unpackInt64();
    }
    
    type = ft02_OrderDesc;

    return true;
  }

  /*! \brief Gets the order type number of this order description.
    \return The order type number.
  */
  unsigned int OrderDescription::getOrderType(){
    return otype;
  }

  /*! \brief Gets the name of this order description.
    /return The name of the order description.
  */
  std::string OrderDescription::getName(){
    return name;
  }

  /*! \brief Gets the text description of this order description.
    /return The text description of the order description.
  */
  std::string OrderDescription::getDescription(){
    return desc;
  }

  /*! \brief Gets a copy of the OrderParameters in this order description.
    \return A copy of the list of OrderParameters.
  */
  std::list<OrderParameter*> OrderDescription::getParameters(){
    std::list<OrderParameter*> np;
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      np.push_back((*itcurr)->clone());
    }
    return np;
  }

    /*! \brief Returns the timestamp of the last time this order description was modified.
    \return The timestamp of the modification time.
    */
    uint64_t OrderDescription::getLastModifiedTime(){
        return modtime;
    }

}

