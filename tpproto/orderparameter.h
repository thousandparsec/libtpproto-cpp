#ifndef TPPROTO_ORDERPARAMETER_H
#define TPPROTO_ORDERPARAMETER_H

/*! \file
  \brief Declares OrderParameter baseclass and the OrderParamType enum.
*/

#include <string>

namespace TPProto{

  /*! \brief Enum of Order parameter type numbers.
   */
  enum OrderParamType{
        opT_Invalid = -1,
        opT_Space_Coord_Abs = 0,
        opT_Time = 1,
        opT_Object_ID = 2,
        opT_Player_ID = 3,
        opT_Space_Coord_Rel = 4,
        opT_Range = 5,
        opT_List = 6,
        opT_String = 7,

        opT_Max
  };

  class Buffer;
  class OrderParameterVisitor;

  /*! \brief A base class for the various types of OrderParamter.
   */
  class OrderParameter{
  public:
    OrderParameter();
    OrderParameter(const OrderParameter &rhs);
    virtual ~OrderParameter();

    /*! \brief Pack the OrderParameter into a Buffer.

    All subclasses must override this method.
    \param buf The Buffer to pack into.
    */
    virtual void packBuffer(Buffer* buf) = 0;

    /*! \brief Unpack the OrderParameter from a Buffer.

    All subclasses must override this method.
    \param buf The Buffer to unpack out of.
    \return True if successful, false otherwise.
    */
    virtual bool unpackBuffer(Buffer* buf) = 0;

    
    /*! \brief Clones the OrderParameter.

    All subclasses must override this method.  They should 
    return a copy of themselves.
    \returns A pointer to a copy of the OrderParameter.
    */
    virtual OrderParameter* clone() = 0;

    /*! \brief Be visisted by an OrderParameterVisitor.

    All subclasses must override this method to be visited
    by the correct method in OrderParameterVisitor.
    \param opv The OrderParameterVisitor that wishes to visit this
    OrderParameter.
    */
    virtual void visit(OrderParameterVisitor* opv) = 0;

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
