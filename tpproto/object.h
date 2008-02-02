#ifndef TPPROTO_OBJECT_H
#define TPPROTO_OBJECT_H
/*  Object - Frame send by the server to tell the client about an object.
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
  \brief Declares Object Frame baseclass.
*/

#include <string>
#include <set>
#include <tpproto/frame.h>
#include <tpproto/vector3d.h>

namespace TPProto{

  class ObjectVisitor;

  /*! \brief The Object Frame baseclass.
    
  All Objects in the game are a subclass of this class.
  */
  class Object : public Frame{
  public:
    virtual ~Object();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    /*! \brief Be visited by an ObjectVisitor.

    This method must be overridden by all subclasses, to call the
    correct visit method in ObjectVisitor.
    \param vis The ObjectVisitor that wants to visit this object.
    */
    virtual void visit(ObjectVisitor * vis) = 0;

    unsigned int getId();
    std::string getName();
    Vector3d getPos();
    Vector3d getVel();
    unsigned int getObjectType();
    uint64_t getSize();
    std::set<unsigned int> getContainedObjectIds();
    std::set<unsigned int> getAvailableOrders();
    unsigned int getNumberOrders();
    uint64_t getLastModifiedTime();
    
    

  protected:
    /*! \brief The Object's Id
     */
    unsigned int id;
    
    /*! \brief The current position of the object.
     */
    Vector3d pos;
    
    /*! \brief The current velocity of the object.
     */
    Vector3d vel;

    /*! \brief The object type number.
     */
    unsigned int obtype;
    
    /*! \brief The diameter of the object in units.
     */
    uint64_t size;
    
    /*! \brief The object's name.
     */
    std::string name;

    /*! \brief The set of objectids that this object contains.
     */
    std::set<unsigned int> contained;

    /*! \brief The set of orderid that can be placed on this object.
     */
    std::set<unsigned int> availableorders;
    
    /*! \brief The number of orders on this object.

    The id of the orders are 0 to numorders-1.
    */
    unsigned int numorders;

    /*! \brief The last modification time of this object.
    */
    uint64_t modtime;

  };

}

#endif
