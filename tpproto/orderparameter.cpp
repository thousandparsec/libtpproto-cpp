

#include "buffer.h"

#include "orderparameter.h"

namespace TPProto{

  /*! \brief Default Constructor.
   */
  OrderParameter::OrderParameter(){
  }

  /*! \brief Copy Constructor.

  Copys the name and description.
  \param rhs Reference to the OrderParameter to be copied.
  */
  OrderParameter::OrderParameter(const OrderParameter& rhs){
    name = rhs.name;
    description = rhs.description;
  }

  /*! \brief Required virtual destructor.
   */
  OrderParameter::~OrderParameter(){
  }

  /*! \brief Gets the name of this OrderParameter.
    \return The name of the OrderParameter.
  */
  std::string OrderParameter::getName(){
    return name;
  }

  /*! \brief Gets the description of this OrderParameter.
    \return The description of the OrderParameter.
  */
  std::string OrderParameter::getDescription(){
    return description;
  }

  /*! \brief Set the name of this OrderParameter.
    \param nn The name to be set.
  */
  void OrderParameter::setName(const std::string &nn){
    name = nn;
  }

  /*! \brief Set the description of this OrderParameter.
    \param nd The description to be set.
  */
  void OrderParameter::setDescription(const std::string & nd){
    description = nd;
  }

}
