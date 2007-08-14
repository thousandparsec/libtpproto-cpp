

#include "silentlogger.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  SilentLogger::SilentLogger(){
  }

  /*! \brief Required virtual destructor.
   */
  SilentLogger::~SilentLogger(){
  }

  /*! \brief Drops error message.
      
    Takes a format string and args.
    \param mesg Message format string to be ignored.
  */
  void SilentLogger::error(const char* mesg, ...){
  }

  /*! \brief Drops a warning message.
      
    Takes a format string and args.
    \param mesg Message format string to be ignored.
  */
  void SilentLogger::warning(const char* mesg, ...){
  }

  /*! \brief Drops an info message.
      
    Takes a format string and args.
    \param mesg Message format string to be ignored.
  */
  void SilentLogger::info(const char* mesg, ...){
  }
  
  /*! \brief Drops a debug message.
      
    Takes a format string and args.
    \param mesg Message format string to be ignored.
  */
  void SilentLogger::debug(const char* mesg, ...){
  }

}

