#ifndef TPPROTO_SILENTLOGGER_H
#define TPPROTO_SILENTLOGGER_H

#include <tpproto/logger.h>

namespace TPProto{

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
