
#include "buffer.h"
#include "orderparametervisitor.h"

#include "stringparameter.h"

namespace TPProto{

  StringParameter::StringParameter(){
  }

  StringParameter::StringParameter(const StringParameter& rhs) : OrderParameter(rhs){
    maxlength = rhs.maxlength;
    value = rhs.value;
  }

  StringParameter::~StringParameter(){
  }

  void StringParameter::packBuffer(Buffer* buf){
    buf->packInt(0);
    buf->packString(value.c_str());
  }

  bool StringParameter::unpackBuffer(Buffer* buf){
    maxlength = buf->unpackInt();
    char* temp = buf->unpackString();
    value = temp;
    delete temp;

    return true;
  }

  OrderParameter* StringParameter::clone(){
    return new StringParameter(*this);
  }

  void StringParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  unsigned int StringParameter::getMaxLength(){
    return maxlength;
  }

  std::string StringParameter::getString(){
    return value;
  }

  void StringParameter::setString(const std::string& nval){
    value = nval;
  }

}
