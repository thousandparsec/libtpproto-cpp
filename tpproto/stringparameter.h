#ifndef TPPROTO_STRINGPARAMETER_H
#define TPPROTO_STRINGPARAMETER_H

#include <string>
#include <tpproto/orderparameter.h>

namespace TPProto{

  class StringParameter : public OrderParameter{
  public:
    StringParameter();
    StringParameter(const StringParameter& rhs);
    ~StringParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    unsigned int getMaxLength();
    std::string getString();

    void setString(const std::string& nval);
    
  private:
    unsigned int maxlength;
    std::string value;
  };

}

#endif
