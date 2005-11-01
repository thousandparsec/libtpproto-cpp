
#include <iostream>

#include <tpproto/framecodec.h>
#include <tpproto/protocollayer.h>

#include "testsocket.h"

using namespace TPProto;

int main(int argc, char** argv){
  int status = 0;

  std::cout << "TPProto library test (simple)" << std::endl;

  FrameCodec *myfc = new FrameCodec();
  myfc->setSocket(new TestSocket());
  myfc->setClientString("SimpleTest");
    ProtocolLayer* layer = new ProtocolLayer();
    layer->setFrameCodec(myfc);

  std::cout << "Test setup complete" << std::endl;

  std::cout << "Starting status: " << myfc->getStatus() << std::endl;

  if(myfc->connect()){
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
