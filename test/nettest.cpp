#include <iostream>

#include <tpproto/framecodec.h>
#include <tpproto/tcpsocket.h>
#include <tpproto/object.h>
#include <tpproto/board.h>

#include "downloadprintvisitor.h"

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

	  std::cout << "Setting up Visitor test" << std::endl;
	  DownloadPrintVisitor* dpv = new DownloadPrintVisitor();
	  dpv->setFrameCodec(myfc);
	  
	  std::cout << "Starting Visitor test" << std::endl;
	  universe->visit(dpv);

	  delete universe;
	  delete dpv;
	  
	  std::cout << "Finished Visitor test, status: " << myfc->getStatus() << std::endl;

	  std::cout << "Starting Board test" << std::endl;

	  Board* myboard = myfc->getPersonalBoard();

	  std::cout << "Got personal board, status " << myfc->getStatus() << std::endl;

	  if(myboard != NULL){

	    std::cout << "Personal board, id: " << myboard->getId() << std::endl << "name: " << myboard->getName()
		      << std::endl << "Desc: " << myboard->getDescription() << std::endl << "Num messages: "
		      << myboard->numMessages() << std::endl;

	    std::cout << "Board test finished" << std::endl;
	    
	    delete myboard;

	  }else{
	    std::cout << "Board test failed, board is NULL, status " << myfc->getStatus() << std::endl;
	    status = 5;
	  }
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
