#ifndef TPPROTO_LOGGER_H
#define TPPROTO_LOGGER_H

namespace TPProto{

  class Logger{
  public:
    virtual ~Logger(){};

    virtual void error(char* mesg, ...) = 0;
    virtual void warning(char* mesg, ...) = 0;
    virtual void info(char* mesg, ...) = 0;
    virtual void debug(char* mesg, ...) = 0;

  };

}

#endif
