#ifndef TPPROTO_LOGGER_H
#define TPPROTO_LOGGER_H

/*! \file
  \brief Declares the Logger interface.
*/

namespace TPProto{

  /*! \brief The logging base class.

  The Logger class is used by the library to write information back to
  the client.  It can be specialised for any application.
  */
  class Logger{
  public:
    /*! \brief Required virtual destructor
     */
    virtual ~Logger(){};

    /*! \brief Log an error message.
      
    Takes a format string and args.
    \param mesg Message format string.
    */
    virtual void error(char* mesg, ...) = 0;

    /*! \brief Log a warning message.
      
    Takes a format string and args.
    \param mesg Message format string.
    */
    virtual void warning(char* mesg, ...) = 0;

    /*! \brief Log an info message.
      
    Takes a format string and args.
    \param mesg Message format string.
    */
    virtual void info(char* mesg, ...) = 0;

    /*! \brief Log a debug message.
      
    Takes a format string and args.
    \param mesg Message format string.
    */
    virtual void debug(char* mesg, ...) = 0;

  };

}

#endif
