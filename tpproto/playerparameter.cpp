

#include "buffer.h"
#include "orderparametervisitor.h"

#include "playerparameter.h"

namespace TPProto{

  PlayerParameter::PlayerParameter(){
  }

  PlayerParameter::PlayerParameter(const PlayerParameter& rhs) : OrderParameter(rhs){
    playerid = rhs.playerid;
    mask = rhs.mask;
  }

  PlayerParameter::~PlayerParameter(){
  }

  void PlayerParameter::packBuffer(Buffer* buf){
    buf->packInt(playerid);
    buf->packInt(0);
  }

  bool PlayerParameter::unpackBuffer(Buffer* buf){
    playerid = buf->unpackInt();
    mask = buf->unpackInt();

    return true;
  }

  OrderParameter* PlayerParameter::clone(){
    return new PlayerParameter(*this);
  }

  void PlayerParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  unsigned int PlayerParameter::getPlayerId(){
    return playerid;
  }

  unsigned int PlayerParameter::getMask(){
    return mask;
  }

  void PlayerParameter::setPlayerId(unsigned int pid){
    playerid = pid;
  }

}

