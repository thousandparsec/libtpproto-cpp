

#include "buffer.h"
#include "orderparametervisitor.h"

#include "timeparameter.h"

namespace TPProto{

  /*! \brief Default constructor.

  Defaults to 0 maxturns and 0 turns.
   */
  TimeParameter::TimeParameter(){
    maxturns = 0;
    turns = 0;
  }

  /*! \brief Copy constructor.
    \param rhs The TimeParameter to copy.
  */
  TimeParameter::TimeParameter(const TimeParameter& rhs) : OrderParameter(rhs){
    maxturns = rhs.maxturns;
    turns = rhs.turns;
  }

  /*! /brief Required virtual destructor.
   */
  TimeParameter::~TimeParameter(){
  }

  /*! \brief Pack into a Buffer.

  The maxturns is packed as zero because it is a read only field.
    \param buf The Buffer to pack into.
  */
  void TimeParameter::packBuffer(Buffer* buf){
    buf->packInt(turns);
    buf->packInt(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool TimeParameter::unpackBuffer(Buffer* buf){
    turns = buf->unpackInt();
    maxturns = buf->unpackInt();

    return true;
  }

  /*! \brief Creates a copy of this TimeParameter.
    \return A new copy of this TimeParameter.
  */
  OrderParameter* TimeParameter::clone(){
    return new TimeParameter(*this);
  }

  /*! \brief Visit this TimeParameter with an OrderParameterVisitor.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void TimeParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the maximum number of turns this parameter can be set to.
    /return The maximum number of turns.
   */
  int TimeParameter::getMaximumTurns(){
    return maxturns;
  }

  /*! \brief Gets the number of turns this parameter is currently set to.
    \return The number of turns.
  */
  int TimeParameter::getTurns(){
    return turns;
  }

  /*! \brief Sets the number of turns.
    /param nt The number of turns.
    \return True if set and is less or equal to the maximum number of turns, 
    false otherwise.
  */
  bool TimeParameter::setTurns(int nt){
    if(nt <= maxturns){
      turns = nt;
      return true;
    }else{
      return false;
    }
  }

}

