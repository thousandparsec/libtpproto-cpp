

#include "buffer.h"
#include "orderparametervisitor.h"

#include "playerparameter.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  PlayerParameter::PlayerParameter(){
  }

  /*! \brief Copy Constructor.
    \param rhs The PlayerParameter to copy.
  */
  PlayerParameter::PlayerParameter(const PlayerParameter& rhs) : OrderParameter(rhs){
    playerid = rhs.playerid;
    mask = rhs.mask;
  }

  /*! \brief Required virtual destructor.
   */
  PlayerParameter::~PlayerParameter(){
  }

  /*! \brief Pack into a Buffer.

  The mask is sent as zero, as per the protocol.
    /param buf The Buffer to pack into.
  */
  void PlayerParameter::packBuffer(Buffer* buf){
    buf->packInt(playerid);
    buf->packInt(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool PlayerParameter::unpackBuffer(Buffer* buf){
    playerid = buf->unpackInt();
    mask = buf->unpackInt();

    return true;
  }

  /*! \brief Clones this PlayerParameter.
    \return A new copy of this PlayerParameter.
  */
  OrderParameter* PlayerParameter::clone(){
    return new PlayerParameter(*this);
  }

  /*! \brief Vists this parameter using an OrderParameterVisitor.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void PlayerParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the current player id currently selected.
    /return The player id.
  */
  unsigned int PlayerParameter::getPlayerId(){
    return playerid;
  }

  /*! \brief Gets the selection mask.
    \return The player mask.
  */
  unsigned int PlayerParameter::getMask(){
    return mask;
  }

  /*! \brief Sets the player id selected.
    \param pid The selected player id.
  */
  void PlayerParameter::setPlayerId(unsigned int pid){
    playerid = pid;
  }

}

