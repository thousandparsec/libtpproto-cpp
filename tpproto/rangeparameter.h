#ifndef TPPROTO_RANGEPARAMETER_H
#define TPPROTO_RANGEPARAMETER_H

/*! \file
  \brief Declares the RangeParameter OrderParamter type.
*/

#include <tpproto/orderparameter.h>

namespace TPProto{

  /*! \brief An OrderParameter that is in a range.
   */
  class RangeParameter : public OrderParameter{
  public:
    RangeParameter();
    RangeParameter(const RangeParameter& rhs);
    ~RangeParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    int getValue();
    int getMinimum();
    int getMaximum();
    int getIncrement();

    bool setValue(int nval);
    
  private:
    int value;
    int min;
    int max;
    int inc;

  };

}

#endif
