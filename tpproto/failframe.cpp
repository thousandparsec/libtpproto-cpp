
#include <cassert>

#include "buffer.h"

#include "failframe.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  FailFrame::FailFrame(){
    errcode = 0;
    errstring = "";
  }

  /*! \brief Required virtual destructor.
   */
  FailFrame::~FailFrame(){

  }

  /*! \brief Assets(0).
    
  This method asserts 0 because it should never be sent to the server.
  \param buf The Buffer (ignored).
  */
  void FailFrame::packBuffer(Buffer *buf){
    // should never send an Fail frame
    assert(0);
  }

  /*! \brief Unpack FailFrame from Buffer.
    
  Unpacks the FrameErrorCode and error string from the buffer.
  \param buf The Buffer to unpack from.
  \return True if successful.
  */
  bool FailFrame::unpackBuffer(Buffer *buf){
    errcode = buf->unpackInt();
    char* temp = buf->unpackString();
    errstring = temp;
    delete[] temp;
    
    type = ft02_Fail;

    return true;
  }

  /*! \brief Gets the error code.
    \return The error code.
  */
  int FailFrame::getErrorCode(){
    return errcode;
  }

  /*! \brief Gets the error string.
    \return The error string.
  */
  std::string FailFrame::getErrorString(){
    return errstring;
  }

}
