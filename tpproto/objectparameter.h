#ifndef TPPROTO_OBJECTPARAMETER_H
#define TPPROTO_OBJECTPARAMETER_H

#include <tpproto/orderparameter.h>

namespace TPProto{

  class ObjectParameter : public OrderParameter{
  public:
    ObjectParameter();
    ObjectParameter(const ObjectParameter& rhs);
    virtual ~ObjectParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();

    unsigned int getObjectId();
    void setObjectId(unsigned int id);

  private:
    unsigned int objectid;

  };

}

#endif
