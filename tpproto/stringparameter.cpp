
#include "buffer.h"
#include "orderparametervisitor.h"

#include "stringparameter.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  StringParameter::StringParameter(){
  }

  /*! \brief Copy constructor.
    \param rhs The StringParameter to copy.
   */
  StringParameter::StringParameter(const StringParameter& rhs) : OrderParameter(rhs){
    maxlength = rhs.maxlength;
    value = rhs.value;
  }

  /*! /brief Required virtual destructor.
   */
  StringParameter::~StringParameter(){
  }

  /*! \brief Pack into a Buffer.

  The maxlength is packed as zero because it is a read only field.
    \param buf The Buffer to pack into.
  */
  void StringParameter::packBuffer(Buffer* buf){
    buf->packInt(0);
    buf->packString(value.c_str());
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */ 
  bool StringParameter::unpackBuffer(Buffer* buf){
    maxlength = buf->unpackInt();
    char* temp = buf->unpackString();
    value = temp;
    delete temp;

    return true;
  }

  /*! \brief Creates a copy of this StringParameter.
    \return A new copy of this StringParameter.
  */
  OrderParameter* StringParameter::clone(){
    return new StringParameter(*this);
  }

  /*! \brief Visit this StringParameter with an OrderParameterVisitor.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void StringParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the maximum length of the string in bytes.
    \return Maximum length of the string in bytes.
  */
  unsigned int StringParameter::getMaxLength(){
    return maxlength;
  }

  /*! \brief Gets the current string parameter.
    \returns The string value.
  */
  std::string StringParameter::getString(){
    return value;
  }

  /*! \brief Sets the string parameter.
    \param nval The string value to use.
  */
  void StringParameter::setString(const std::string& nval){
    value = nval;
  }

}
