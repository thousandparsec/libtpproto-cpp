
#include "buffer.h"
#include "orderparametervisitor.h"

#include "rangeparameter.h"

namespace TPProto{

  RangeParameter::RangeParameter(){
  }

  RangeParameter::RangeParameter(const RangeParameter& rhs) : OrderParameter(rhs){
    value = rhs.value;
    min = rhs.min;
    max = rhs.max;
    inc = rhs.inc;
  }

  RangeParameter::~RangeParameter(){
  }

  void RangeParameter::packBuffer(Buffer* buf){
    buf->packInt(value);
    buf->packInt(0);
    buf->packInt(0);
    buf->packInt(0);
  }

  bool RangeParameter::unpackBuffer(Buffer* buf){
    value = buf->unpackInt();
    min = buf->unpackInt();
    max = buf->unpackInt();
    inc = buf->unpackInt();
    return true;
  }

  OrderParameter* RangeParameter::clone(){
    return new RangeParameter(*this);
  }

  void RangeParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  int RangeParameter::getValue(){
    return value;
  }

  int RangeParameter::getMinimum(){
    return min;
  }

  int RangeParameter::getMaximum(){
    return max;
  }

  int RangeParameter::getIncrement(){
    return inc;
  }

  bool RangeParameter::setValue(int nval){
    if(nval >= min && nval <= max){
      value = nval;
      return true;
    }
    return false;
  }

}

