/*  ProbeOrder frame class
 *
 *  Copyright (C) 2005  Lee Begg and the Thousand Parsec Project
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
#include "orderparameter.h"
#include "orderdesc.h"

#include "probeorder.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    ProbeOrder::ProbeOrder(){
    }

  /*! \brief Destructor.

  Deletes the OrderParameters it contains.
  */
  ProbeOrder::~ProbeOrder(){
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      delete *itcurr;
    }
    params.clear();
  }

  /*! \brief Pack this ProbeOrder Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void ProbeOrder::packBuffer(Buffer* buf){
    buf->packInt(object);
    buf->packInt(slot);
    buf->packInt(desc->getOrderType());
    buf->packInt(0); // numturns
    buf->packInt(0); // resource list

    //pack paramters
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      (*itcurr)->packBuffer(buf);
    }

    type = ft03_Order_Probe;
  }

  /*! \brief Unpack this ProbeOrder from a Buffer.
    Since the probe order cannot be received by the client, this method returns false.
    \param buf The Buffer to unpack out of.
    \return False always.
  */
  bool ProbeOrder::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Gets the name of this order type.
    \return The name of the order.
  */
  std::string ProbeOrder::getName(){
    return desc->getName();
  }

  /*! \brief Gets the text description of this order type.
    \return The text description of the order.
  */
  std::string ProbeOrder::getDescription(){
    return desc->getDescription();
  }

  /*! \brief Gets the order type number of this order.
    \return The order type number.
  */
  unsigned int ProbeOrder::getOrderType(){
    return desc->getOrderType();
  }

  /*! \brief Gets the Object id this order is on.
    \return The object id.
  */
  unsigned int ProbeOrder::getObjectId(){
    return object;
  }

  /*! \brief Gets the slot number for this order.

  Range starts at 0. -1 means the last slot.
  \return The slot number.
  */
  int ProbeOrder::getSlot(){
    return slot;
  }

  /*! \brief Gets the number of turns until this order is completed.
    \return The number of turns before completion.
  */
  unsigned int ProbeOrder::getNumTurns(){
    return numturns;
  }

  /*! \brief Gets the number of OrderParameters this order has.
    \return The number of parameters.
  */
  unsigned int ProbeOrder::getNumParameters(){
    return params.size();
  }

  /*! \brief Gets an OrderParameter.
    \param which The number of the parameter to get.
    \return The which-th OrderParameter on this order or NULL if
    which is not valid.
  */
  OrderParameter* ProbeOrder::getParameter(unsigned int which){
    if(which < params.size()){
      std::list<OrderParameter*>::iterator itcurr = params.begin();
      advance(itcurr, which);
      if(itcurr != params.end()){
	return *itcurr;
      }
    }
    
    return NULL;
    
  }

  /*! \brief Sets the Object id this order will be on.
    \param oid The object id.
  */
  void ProbeOrder::setObjectId(unsigned int oid){
    object = oid;
  }

  /*! \brief Sets the slot number the order will be placed.

  Starts from 0. -1 is the last slot.
  \param s The slot number.
  */
  void ProbeOrder::setSlot(int s){
    slot = s;
  }

  /*! \brief Sets the order type using an OrderDescription.
    \param od The OrderDescription describing the order type required.
  */
  void ProbeOrder::setOrderType(OrderDescription* od){
    desc = od;
    params = od->getParameters();
  }

}

