
#include <cassert>

#include "buffer.h"

#include "okframe.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  OkFrame::OkFrame(){
    info = "";
  }

  /*! \brief Required virtual destructor.
   */
  OkFrame::~OkFrame(){

  }

  /*! \brief Pack into a Buffer, Assert(0).
    
  This Frame does not get sent to the server, so this method asserts zero.
  \param buf Buffer to pack into, ignored.
  */ 
  void OkFrame::packBuffer(Buffer *buf){
    // should never send an OK frame
    assert(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack out of.
    \return True if successful, false otherwise.
  */
  bool OkFrame::unpackBuffer(Buffer *buf){
    char* temp = buf->unpackString();
    info = temp;
    delete[] temp;
    
    type = ft02_OK;

    return true;
  }

  /*! \brief Gets the informational string sent in the OKFrame.
    \return The informational string.
  */
  std::string OkFrame::getInfo(){
    return info;
  }

}
