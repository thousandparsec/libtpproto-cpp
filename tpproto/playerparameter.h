#ifndef TPPROTO_PLAYERPARAMETER_H
#define TPPROTO_PLAYERPARAMETER_H

#include <tpproto/orderparameter.h>

namespace TPProto{

  class PlayerParameter : public OrderParameter{
  public:
    PlayerParameter();
    PlayerParameter(const PlayerParameter& rhs);
    ~PlayerParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    unsigned int getPlayerId();
    unsigned int getMask();

    void setPlayerId(unsigned int pid);
    
  private:
    unsigned int playerid;
    unsigned int mask;

  };

}

#endif
