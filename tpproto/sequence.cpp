
#include <cassert>

#include "buffer.h"

#include "sequence.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  Sequence::Sequence(){

  }

  /*! \brief Required virtual destructor.
   */
  Sequence::~Sequence(){

  }

  /*! \brief Pack into a Buffer, Assert(0).
    
  This Frame does not get sent to the server, so this method asserts zero.
  \param buf Buffer to pack into, ignored.
  */ 
  void Sequence::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool Sequence::unpackBuffer(Buffer* buf){
    num = buf->unpackInt();
    type = ft02_Sequence;
    return true;
  }

  /*! \brief Gets the number of Frames that should follow this one.
    \return The number of Frames.
  */
  int Sequence::getNumber(){
    return num;
  }

}

