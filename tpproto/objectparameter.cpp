

#include "buffer.h"
#include "orderparametervisitor.h"

#include "objectparameter.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  ObjectParameter::ObjectParameter(){
  }

  /*! \brief Copy constructor.
    \param rhs The ObjectParameter to copy.
  */
  ObjectParameter::ObjectParameter(const ObjectParameter& rhs) : OrderParameter(rhs){
    objectid = rhs.objectid;
  }

  /*! \brief Required virtual destructor.
   */
  ObjectParameter::~ObjectParameter(){
  }

  /*! \brief Pack into a Buffer.
    \param buf The Buffer to pack into.
  */
  void ObjectParameter::packBuffer(Buffer* buf){
    buf->packInt(objectid);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool ObjectParameter::unpackBuffer(Buffer* buf){
    objectid = buf->unpackInt();

    return true;
  }

  /*! \brief Clone this ObjectParameter.
    \return A new copy of this ObjectParameter.
  */
  OrderParameter* ObjectParameter::clone(){
    return new ObjectParameter(*this);
  }

  /*! \brief Visit this ObjectParameter.
    \param opv The OrderParameterVisitor that wishes to visit.
  */
  void ObjectParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  /*! \brief Gets the Objectid this parameter currently points to.
    \return The objectid.
  */
  unsigned int ObjectParameter::getObjectId(){
    return objectid;
  }

  /*! \brief Sets the object id this parameter points to.
    \param id The object id that his parameter should refer to.
  */
  void ObjectParameter::setObjectId(unsigned int id){
    objectid = id;
  }

}
