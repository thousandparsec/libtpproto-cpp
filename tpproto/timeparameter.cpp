

#include "buffer.h"
#include "orderparametervisitor.h"

#include "timeparameter.h"

namespace TPProto{

  TimeParameter::TimeParameter(){
    maxturns = 0;
    turns = 0;
  }

  TimeParameter::TimeParameter(const TimeParameter& rhs) : OrderParameter(rhs){
    maxturns = rhs.maxturns;
    turns = rhs.turns;
  }

  TimeParameter::~TimeParameter(){
  }

  void TimeParameter::packBuffer(Buffer* buf){
    buf->packInt(turns);
    buf->packInt(0);
  }

  bool TimeParameter::unpackBuffer(Buffer* buf){
    turns = buf->unpackInt();
    maxturns = buf->unpackInt();

    return true;
  }

  OrderParameter* TimeParameter::clone(){
    return new TimeParameter(*this);
  }

  void TimeParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  int TimeParameter::getMaximumTurns(){
    return maxturns;
  }

  int TimeParameter::getTurns(){
    return turns;
  }

  bool TimeParameter::setTurns(int nt){
    if(nt <= maxturns){
      turns = nt;
      return true;
    }else{
      return false;
    }
  }

}

