#ifndef TPPROTO_TIMEPARAMETER_H
#define TPPROTO_TIMEPARAMETER_H

#include <tpproto/orderparameter.h>

namespace TPProto{

  class TimeParameter : public OrderParameter{
  public:
    TimeParameter();
    TimeParameter(const TimeParameter& rhs);
    virtual ~TimeParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();

    int getMaximumTurns();
    int getTurns();

    bool setTurns(int nt);

  private:
    int maxturns;
    int turns;

  };

}

#endif
