#ifndef TPPROTO_ORDERPARAMETER_H
#define TPPROTO_ORDERPARAMETER_H

#include <string>

namespace TPProto{

  class Buffer;

  class OrderParameter{
  public:
    OrderParameter(const OrderParameter &rhs);
    virtual ~OrderParameter();

    virtual void packBuffer(Buffer* buf) = 0;
    virtual bool unpackBuffer(Buffer* buf) = 0;

    virtual OrderParameter* clone() = 0;

    std::string getName();
    std::string getDescription();
    
    void setName(const std::string &nn);
    void setDescription(const std::string &nd);

  private:
    std::string name;
    std::string description;

  };

}

#endif
