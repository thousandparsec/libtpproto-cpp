#include <iostream>

#include <tpproto/framecodec.h>
#include <tpproto/tcpsocket.h>
#include <tpproto/object.h>

using namespace TPProto;

int main(int argc, char** argv){
  int status = 0;
  
  std::cout << "TPProto library test (network)" << std::endl;
  
  if(argc < 2){
    
    std::cerr << "Need at least one argument" << std::endl;
    std::cerr << "nettest <hostname-or-ip> [<port> [<username> <password>]]" << std::endl;

    status = 1;
  }else{

    TcpSocket * ts = new TcpSocket();
    if(argc == 2)
      ts->setServerAddr(argv[1]);
    else
      ts->setServerAddr(argv[1], argv[2]);

    FrameCodec *myfc = new FrameCodec();
    myfc->setSocket(ts);
    myfc->setClientString("libtpproto-cpp_NetTest");

    std::cout << "Test setup complete" << std::endl;

    std::cout << "Starting status: " << myfc->getStatus() << std::endl;

    if(myfc->connect()){
      std::cout << "Connected ok, status: " << myfc->getStatus() << std::endl;
      
      if(myfc->login("test", "test")){
	std::cout << "Login ok, status: " << myfc->getStatus() << std::endl;

	Object* universe = myfc->getUniverse();
	if(universe != NULL){
	  
	  std::cout << "Got the Universe object ok, status: " << myfc->getStatus() << std::endl;


	}else{
	  std::cout << "Failed to get Universe Object, status" << myfc->getStatus() << std::endl;
	  status = 4;
	}
	
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
    
    
  }
  
  return status;
}
