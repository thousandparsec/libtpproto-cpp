

#include "buffer.h"
#include "orderparametervisitor.h"

#include "objectparameter.h"

namespace TPProto{

  ObjectParameter::ObjectParameter(){
  }

  ObjectParameter::ObjectParameter(const ObjectParameter& rhs) : OrderParameter(rhs){
    objectid = rhs.objectid;
  }

  ObjectParameter::~ObjectParameter(){
  }

  void ObjectParameter::packBuffer(Buffer* buf){
    buf->packInt(objectid);
  }

  bool ObjectParameter::unpackBuffer(Buffer* buf){
    objectid = buf->unpackInt();

    return true;
  }

  OrderParameter* ObjectParameter::clone(){
    return new ObjectParameter(*this);
  }

  void ObjectParameter::visit(OrderParameterVisitor* opv){
    opv->visitOrderParam(this);
  }

  unsigned int ObjectParameter::getObjectId(){
    return objectid;
  }

  void ObjectParameter::setObjectId(unsigned int id){
    objectid = id;
  }

}
