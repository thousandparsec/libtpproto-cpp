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

    void error(const char* mesg, ...);
    void warning(const char* mesg, ...);
    void info(const char* mesg, ...);
    void debug(const char* mesg, ...);

  };

}

#endif
