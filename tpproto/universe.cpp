

#include "buffer.h"
#include "objectvisitor.h"

#include "universe.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  Universe::Universe(){

  }

  /*! \brief Required virtual destructor.
   */
  Universe::~Universe(){

  }

  /*! \brief Unpack extra data from Buffer.

  First calls Object::unpackBuffer() and then unpacks the age of the universe
  from the Buffer.
  \param buf The Buffer to unpack from.
  \return True if successful.
  */
  bool Universe::unpackBuffer(Buffer *buf){
    Object::unpackBuffer(buf);
    age = buf->unpackInt();
    return true;
  }

  /*! \brief Visit the Universe with an ObjectVisitor.
    \param vis The ObjectVisitor that wishes to visit.
  */
  void Universe::visit(ObjectVisitor * vis){
    vis->visit(this);
  }

  /*! \brief Gets the age of the universe.
    /return The age of the universe in turns (years).
  */
  unsigned int Universe::getAge(){
    return age;
  }

}
