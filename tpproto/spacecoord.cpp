

#include "buffer.h"
#include "orderparametervisitor.h"

#include "spacecoord.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  SpaceCoordinates::SpaceCoordinates(){
  }

  /*! \brief Copy constructor.
    \param rhs The SpaceCoordinates to copy from.
  */
  SpaceCoordinates::SpaceCoordinates(const SpaceCoordinates& rhs) : OrderParameter(rhs){
    pos = rhs.pos;
  }

  /*! \brief Required virtual destructor.
   */
  SpaceCoordinates::~SpaceCoordinates(){
  }

  /*! \brief Pack into a Buffer.
    \param buf The Buffer to pack into.
  */
  void SpaceCoordinates::packBuffer(Buffer* buf){
    pos.pack(buf);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool SpaceCoordinates::unpackBuffer(Buffer* buf){
    pos.unpack(buf);
    return true;
  }

  /*! \brief Creates a copy of this SpaceCoordinates.
    \return A new copy of these SpaceCoordinates.
  */
  OrderParameter* SpaceCoordinates::clone(){
    return new SpaceCoordinates(*this);
  }

  /*! \brief Visit this SpaceCoordinates with an OrderParameterVisitor.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void SpaceCoordinates::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the position.
    \return The Vector3d of the position.
  */
  Vector3d SpaceCoordinates::getPos(){
    return pos;
  }

  /*! \brief Sets the position.
    \param npos The Vector3d position to be set.
  */
  void SpaceCoordinates::setPos(const Vector3d& npos){
    pos = npos;
  }

}

