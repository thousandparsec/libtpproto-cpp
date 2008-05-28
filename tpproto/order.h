#ifndef TPPROTO_ORDER_H
#define TPPROTO_ORDER_H
/*  Order - Frame send by the server to tell the client about an order.
 *
 *  Copyright (C) 2004, 2005, 2008  Lee Begg and the Thousand Parsec Project
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
  \brief Declares Order Frame.
*/

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>

#include <tpproto/frame.h>

namespace TPProto{

  class OrderParameter;
  class OrderDescription;

  /*! \brief A Frame representing an Order on an Object.
   */
  class Order : public Frame{
  public:
    Order();
    virtual ~Order();
    
    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    std::string getName();
    std::string getDescription();
    unsigned int getOrderType();

    unsigned int getObjectId();
    int getSlot();
    unsigned int getNumTurns();

    unsigned int getNumParameters();
    OrderParameter* getParameter(unsigned int which);

    void setObjectId(unsigned int oid);
    void setSlot(int s);
    void setOrderType(boost::shared_ptr<OrderDescription> od);

  private:
    unsigned int object;
    int slot;
    unsigned int otype;
    unsigned int numturns;

    std::list<OrderParameter*> params;

    boost::shared_ptr<OrderDescription> desc;

  };

}

#endif
