
#include "buffer.h"
#include "objectvisitor.h"

#include "planet.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  Planet::Planet(){
  }

  /*! \brief Required virtual destructor.
   */
  Planet::~Planet(){
  }

  /*! \brief Unpack extra data from Buffer.

  First calls Object::unpackBuffer() and then unpacks the owner playerid from
  the Buffer.
  \param buf The Buffer to unpack from.
  \return True if successful.
  */
  bool Planet::unpackBuffer(Buffer* buf){
    Object::unpackBuffer(buf);
    owner = buf->unpackInt();
    return true;
  }

  /*! \brief Visit this Planet with an ObjectVisitor.
    \param vis The ObjectVisitor that wishes to visit.
  */
  void Planet::visit(ObjectVisitor* vis){
    vis->visit(this);
  }
  
  /*! \brief Gets the playerid of the owner of this Planet.
    \return The playerid of the owner.
  */
  int Planet::getOwner(){
    return owner;
  }

}
