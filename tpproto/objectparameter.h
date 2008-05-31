#ifndef TPPROTO_OBJECTPARAMETER_H
#define TPPROTO_OBJECTPARAMETER_H
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


/*! \file
  \brief Declares ObjectParameter baseclass and the ObjectParamType enum.
*/

#include <string>

namespace TPProto{

  /*! \brief Enum of Order parameter type numbers.
   */
  enum ObjectParamType{
        obpT_Invalid = -1,
        obpT_Position_3D = 0,
        obpT_Velocity_3D = 1,
        obpT_Acceleration_3D = 2,
        obpT_Position_Bound = 3,
        obpT_Order_Queue = 4,
        obpT_Resource_List = 5,
        obpT_Reference = 6,
        obpT_Reference_Quantity_List = 7,
        obpT_Integer = 8,
        obpT_Size = 9,
        obpT_Media = 10,

        obpT_Max
  };

  class Buffer;
  class ObjectParameterVisitor;

  /*! \brief A base class for the various types of ObjectParameter.
   */
  class ObjectParameter{
  public:
    ObjectParameter();
    ObjectParameter(const ObjectParameter &rhs);
    virtual ~ObjectParameter();

    /*! \brief Pack the ObjectParameter into a Buffer.

    All subclasses must override this method.
    \param buf The Buffer to pack into.
    */
    virtual void packBuffer(Buffer* buf) = 0;

    /*! \brief Unpack the ObjectParameter from a Buffer.

    All subclasses must override this method.
    \param buf The Buffer to unpack out of.
    \return True if successful, false otherwise.
    */
    virtual bool unpackBuffer(Buffer* buf) = 0;

    /*! \brief Unpack the ObjectParameter's description from a Buffer.
    
    Used to get the data in the ObjectDescription frame. All subclasses
    must override this method.
    \param buf The Buffer to unpack out of.
    \return True if successful, false otherwise.
    */
    virtual bool unpackDescBuffer(Buffer* buf) = 0;
    
    /*! \brief Clones the ObjectParameter.

    All subclasses must override this method.  They should 
    return a copy of themselves.
    \returns A pointer to a copy of the ObjectParameter.
    */
    virtual ObjectParameter* clone() = 0;

    /*! \brief Be visisted by an ObjectParameterVisitor.

    All subclasses must override this method to be visited
    by the correct method in ObjectParameterVisitor.
    \param opv The ObjectParameterVisitor that wishes to visit this
    ObjectParameter.
    */
    virtual void visit(ObjectParameterVisitor* opv) = 0;

    std::string getName();
    std::string getDescription();
    
    void setName(const std::string &nn);
    void setDescription(const std::string &nd);

  private:
    std::string name;
    std::string description;

  };

}

#endif
