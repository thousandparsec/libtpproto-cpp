

#include "buffer.h"
#include "orderparametervisitor.h"

#include "spacecoordrel.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  SpaceCoordinatesRel::SpaceCoordinatesRel(){
  }

  /*! \brief Copy constructor.
    \param rhs The SpaceCoordinatesRel to copy from.
  */
  SpaceCoordinatesRel::SpaceCoordinatesRel(const SpaceCoordinatesRel& rhs) : OrderParameter(rhs){
    pos = rhs.pos;
  }

  /*! \brief Required virtual destructor.
   */
  SpaceCoordinatesRel::~SpaceCoordinatesRel(){
  }

  /*! \brief Pack into a Buffer.
    \param buf The Buffer to pack into.
  */
  void SpaceCoordinatesRel::packBuffer(Buffer* buf){
    buf->packInt(objectid);
    pos.pack(buf);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */ 
  bool SpaceCoordinatesRel::unpackBuffer(Buffer* buf){
    objectid = buf->unpackInt();
    pos.unpack(buf);
    return true;
  }

  /*! \brief Creates a copy of this SpaceCoordinatesRel.
    \return A new copy of these SpaceCoordinatesRel.
  */
  OrderParameter* SpaceCoordinatesRel::clone(){
    return new SpaceCoordinatesRel(*this);
  }

  /*! \brief Visit this SpaceCoordinatesRel with an OrderParameterVisitor.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void SpaceCoordinatesRel::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the position relative to the Object.
    \return The Vector3d of the position vector.
  */
  Vector3d SpaceCoordinatesRel::getPos(){
    return pos;
  }

  /*! \brief Gets the Object id for the relative position.
    \return The object id.
  */
  unsigned int SpaceCoordinatesRel::getObjectId(){
    return objectid;
  }

  /*! \brief Sets the relative position.
    \param npos The Vector3d position to be set.
  */
  void SpaceCoordinatesRel::setPos(const Vector3d& npos){
    pos = npos;
  }

  /*! \brief Sets the Object id for the relative position.
    \param oid The object id.
  */
  void SpaceCoordinatesRel::setObjectId(unsigned int oid){
    objectid = oid;
  }

}

