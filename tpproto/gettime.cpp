

#include "buffer.h"

#include "gettime.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  GetTime::GetTime(){
  }

  /*! \brief Required virtual destructor.
   */
  GetTime::~GetTime(){
  }

  /*! \brief Pack this GetTime Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void GetTime::packBuffer(Buffer* buf){
    type = ft02_Time_Remaining_Get;
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since GetTime frames are only sent to the server, we should not 
  receive them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool GetTime::unpackBuffer(Buffer* buf){
    return false;
  }

}

