
#include <cassert>

#include "buffer.h"

#include "timeremaining.h"

namespace TPProto{
 
  /*! \brief Default constructor.
   */
  TimeRemaining::TimeRemaining(){
  }
  
  /*! \brief Required virtual destructor.
   */
  TimeRemaining::~TimeRemaining(){
  }

  /*! \brief Pack into a Buffer, Assert(0).
    
  This Frame does not get sent to the server, so this method asserts zero.
  \param buf Buffer to pack into, ignored.
  */ 
  void TimeRemaining::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpack from a Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
  */
  bool TimeRemaining::unpackBuffer(Buffer* buf){
    time = buf->unpackInt();
    type = ft02_Time_Remaining;
    return true;
  }

  /*! \brief Gets the time remaining before the end of turn.
    \return The time in seconds before the end of turn.
  */
  int TimeRemaining::getTimeRemaining(){
    return time;
  }

}

