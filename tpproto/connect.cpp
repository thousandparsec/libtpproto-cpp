

#include "buffer.h"

#include "connect.h"

namespace TPProto{

  /* \brief Default constructor.
   */
  Connect::Connect(){
    clientstring = "";
  }

  /*! \brief Destructor.
   */
  Connect::~Connect(){
  }

  /*! \brief Packs the Connect Frame into a buffer.
    
  This method packs the client string into the buffer and makes sure the
  type of this frame is set correctly.
  \param buf The Buffer to pack the data into.
  */
  void Connect::packBuffer(Buffer* buf){
    buf->packString(clientstring.c_str());
    type = ft02_Connect;
  }

  /*! \brief Unpacks data from the buffer.
    
  Always returns false because we should never receive one.
  \return False always.
  */
  bool Connect::unpackBuffer(Buffer* buf){
    // should never receive this frame
    return false;
  }

  /*! \brief Sets the client string.
    
  The client string is sent in the Connect Frame.  The perferred method is
  using the method provided by FrameCodec (which eventually calls this one).
  \param cs The string to be set as the client string.
  */
  void Connect::setClientString(const std::string &cs){
    clientstring = cs;
  }

}
