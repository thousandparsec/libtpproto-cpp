
#include <iostream>
#include <stdarg.h>

#include <tpproto/gamelayer.h>
#include <tpproto/logger.h>

#include "testsocket.h"

using namespace TPProto;

class StdoutLogger : public Logger{
    public:
        StdoutLogger(){}
        virtual ~StdoutLogger(){}

        void error(char* mesg, ...){
            printf("[error] ");
            va_list ap;
            va_start(ap, mesg);
            vprintf(mesg, ap);
            va_end(ap);
            printf("\n");
        }
        void warning(char* mesg, ...){
            printf("[warning] ");
            va_list ap;
            va_start(ap, mesg);
            vprintf(mesg, ap);
            va_end(ap);
            printf("\n");
        }
        void info(char* mesg, ...){
            printf("[info] ");
            va_list ap;
            va_start(ap, mesg);
            vprintf(mesg, ap);
            va_end(ap);
            printf("\n");
        }
        void debug(char* mesg, ...){
            printf("[debug] ");
            va_list ap;
            va_start(ap, mesg);
            vprintf(mesg, ap);
            va_end(ap);
            printf("\n");
        }

};

int main(int argc, char** argv){
  int status = 0;

  std::cout << "TPProto library test (simple)" << std::endl;

    GameLayer *myfc = new GameLayer();
    myfc->setClientString("SimpleTest");
    myfc->setLogger(new StdoutLogger());

  std::cout << "Test setup complete" << std::endl;

  std::cout << "Starting status: " << myfc->getStatus() << std::endl;

  if(myfc->connect(new TestSocket())){
    std::cout << "Connected ok, status: " << myfc->getStatus() << std::endl;

    if(myfc->login("test", "test")){
      std::cout << "Login ok, status: " << myfc->getStatus() << std::endl;

    }else{
      std::cout << "Failed to login, status: " << myfc->getStatus() << std::endl;
      status = 2;
    }

    myfc->disconnect();
    if(myfc->getStatus() != 0){
      status = 3;
    }

    std::cout << "Disconnected, status: " << myfc->getStatus() << std::endl;

  }else{
    std::cout << "Failed to Connect, status: " << myfc->getStatus() << std::endl;
    status = 1;
  }

  std::cout << "Cleaning up after tests" << std::endl;
  delete myfc;
  std::cout << "Clean up done, exiting" << std::endl;

  return status;

}
