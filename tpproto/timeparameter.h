#ifndef TPPROTO_TIMEPARAMETER_H
#define TPPROTO_TIMEPARAMETER_H

/*! \file
  \brief Declares TimeParameter OrderParameter type.
*/

#include <tpproto/orderparameter.h>

namespace TPProto{

  /*! \brief A time OrderParameter with maximum number of turns.
   */
  class TimeParameter : public OrderParameter{
  public:
    TimeParameter();
    TimeParameter(const TimeParameter& rhs);
    virtual ~TimeParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    int getMaximumTurns();
    int getTurns();

    bool setTurns(int nt);

  private:
    int maxturns;
    int turns;

  };

}

#endif
