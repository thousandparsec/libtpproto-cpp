

#include "buffer.h"

#include "orderparameter.h"

namespace TPProto{

  OrderParameter::OrderParameter(const OrderParameter& rhs){
    name = rhs.name;
    description = rhs.description;
  }

  OrderParameter::~OrderParameter(){
  }

  std::string OrderParameter::getName(){
    return name;
  }

  std::string OrderParameter::getDescription(){
    return description;
  }

  void OrderParameter::setName(const std::string &nn){
    name = nn;
  }

  void OrderParameter::setDescription(const std::string & nd){
    description = nd;
  }

}
