#ifndef TPPROTO_ORDERPARAMETER_H
#define TPPROTO_ORDERPARAMETER_H

#include <string>

namespace TPProto{

  enum OrderParamType{
        opT_Invalid = -1,
        opT_Space_Coord_Abs = 0,
        opT_Time = 1,
        opT_Object_ID = 2,
        opT_Player_ID = 3,
        opT_Space_Coord_Rel = 4,
        opT_Range = 5,
        opT_List = 6,
        opT_String = 7,

        opT_Max
  };

  class Buffer;

  class OrderParameter{
  public:
    OrderParameter();
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
