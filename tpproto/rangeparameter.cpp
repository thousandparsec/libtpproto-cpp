
#include "buffer.h"
#include "orderparametervisitor.h"

#include "rangeparameter.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  RangeParameter::RangeParameter(){
  }

  /*! \brief Copy constructor.
    \param rhs The RanageParameter to copy.
  */
  RangeParameter::RangeParameter(const RangeParameter& rhs) : OrderParameter(rhs){
    value = rhs.value;
    min = rhs.min;
    max = rhs.max;
    inc = rhs.inc;
  }

  /*! \brief Required virtual destructor.
   */
  RangeParameter::~RangeParameter(){
  }

  /*! \brief Pack the RangeParameter in a Buffer.
    
  Only the value is packed, along with three ints padding, as they are 
  read only fields.
  /param buf The Buffer to pack into.
  */
  void RangeParameter::packBuffer(Buffer* buf){
    buf->packInt(value);
    buf->packInt(0);
    buf->packInt(0);
    buf->packInt(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool RangeParameter::unpackBuffer(Buffer* buf){
    value = buf->unpackInt();
    min = buf->unpackInt();
    max = buf->unpackInt();
    inc = buf->unpackInt();
    return true;
  }

  /*! \brief Create a copy of this RangeParameter.
    \return A new copy of this RangeParameter.
  */
  OrderParameter* RangeParameter::clone(){
    return new RangeParameter(*this);
  }

  /*! \brief Visit this parameter using an OrderParameterVisitor.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void RangeParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the current value of this parameter.
    \return The current value.
  */
  int RangeParameter::getValue(){
    return value;
  }

  /*! \brief Gets the Minimum value for this parameter.
    \return The minimum value.
  */
  int RangeParameter::getMinimum(){
    return min;
  }

  /*! \brief Gets the Maximum value for this parameter.
    \return The maximum value.
  */
  int RangeParameter::getMaximum(){
    return max;
  }

  /*! \brief Gets the increment the value can have.
    \return The increment.
  */
  int RangeParameter::getIncrement(){
    return inc;
  }

  /*! \brief Sets the value of this parameter.
    \param nval The value for this parameter.
    \return True if set and was in range, otherwise false.
  */
  bool RangeParameter::setValue(int nval){
    if(nval >= min && nval <= max){
      value = nval;
      return true;
    }
    return false;
  }

}

