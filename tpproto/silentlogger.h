#ifndef TPPROTO_SILENTLOGGER_H
#define TPPROTO_SILENTLOGGER_H

/*! \file
  \brief Declares SilentLogger.
*/

#include <tpproto/logger.h>

namespace TPProto{

  /*! \brief A Logger that silently discards all messages.
   */
  class SilentLogger : public Logger{
  public:
    SilentLogger();
    virtual ~SilentLogger();

    void error(char* mesg, ...);
    void warning(char* mesg, ...);
    void info(char* mesg, ...);
    void debug(char* mesg, ...);

  };

}

#endif
