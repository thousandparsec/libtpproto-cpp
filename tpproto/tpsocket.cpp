

#include "tpsocket.h"

namespace TPProto{
  
  /*! \brief Required virtual destructor.
   */
  TPSocket::~TPSocket(){}
  
  /*! \brief Checks if the socket is connected.

  Subclasses should override this method and track whether they 
  are connected or not.  Default always returns false.
  \return True if connected, false otherwise.
  */
  bool TPSocket::isConnected(){
    return false;
  }

  /*! \brief Poll the socket for data.
      
    If a socket does not support polling, it should always return false
    and does not need to override this method as the default is to
    return false.
    \return True if there is data wait, false otherwise.
  */
  bool TPSocket::poll(){
    return false;
  }
  
}
