#ifndef TPPROTO_OBJECTPARAMETER_H
#define TPPROTO_OBJECTPARAMETER_H

/*! \file
  \brief Declares ObjectParameter OrderParameter.
*/

#include <tpproto/orderparameter.h>

namespace TPProto{

  /*! \brief An OrderParameter that refers to an Object.
   */
  class ObjectParameter : public OrderParameter{
  public:
    ObjectParameter();
    ObjectParameter(const ObjectParameter& rhs);
    virtual ~ObjectParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    unsigned int getObjectId();
    void setObjectId(unsigned int id);

  private:
    unsigned int objectid;

  };

}

#endif
