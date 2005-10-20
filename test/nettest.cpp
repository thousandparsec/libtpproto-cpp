#include <iostream>
#include <stdarg.h>

#include <tpproto/framecodec.h>
#include <tpproto/tcpsocket.h>
#include <tpproto/object.h>
#include <tpproto/board.h>
#include <tpproto/message.h>
#include <tpproto/getmessage.h>
#include <tpproto/removemessage.h>
#include <tpproto/order.h>
#include <tpproto/getorder.h>
#include <tpproto/removeorder.h>
#include <tpproto/timeparameter.h>
#include <tpproto/logger.h>

#include "downloadprintvisitor.h"
#include "printasynclistener.h"

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
    myfc->setLogger(new StdoutLogger());
    myfc->setSocket(ts);
    myfc->setClientString("libtpproto-cpp_NetTest");

    PrintAsyncListener* pal = new PrintAsyncListener();
    myfc->setAsyncFrameListener(pal);

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

	  int pob = dpv->getPlayableObject();
	  delete dpv;

	  std::cout << "Finished Visitor test, status: " << myfc->getStatus() << std::endl;
	  
	  std::cout << "Starting Order test" << std::endl;
	  
	  Order* order = myfc->createOrderFrame(0);
	  order->setObjectId(pob);
	  order->setSlot(0);
	  if(myfc->insertOrder(order)){
	    std::cout << "Successfully added order, status " << myfc->getStatus() << std::endl;
	    //delete order;
	    
	    GetOrder* gor = myfc->createGetOrderFrame();
	    gor->addOrderId(0);
	    gor->setObjectId(pob);
	    std::map<unsigned int, Order*> orlist = myfc->getOrders(gor);
	    //delete gor;
	    order = orlist.begin()->second;
	    std::cout << "Order: slot " << order->getSlot() << std::endl;
	    std::cout << "obid: " << order->getObjectId() << std::endl;
	    std::cout << "type: " << order->getOrderType() << std::endl;
	    std::cout << "num turns: " << order->getNumTurns() << std::endl;
	    std::cout << "num params: " << order->getNumParameters() << std::endl;

	    //mod order
	    TimeParameter* timep = (TimeParameter*)order->getParameter(0);
	    timep->setTurns(10);
	    
	    if(myfc->replaceOrder(order)){
	      std::cout << "Successfully replaced order, status " << myfc->getStatus() << std::endl;
	      
	      //delete order;
	      
	      gor = myfc->createGetOrderFrame();
	      gor->addOrderRange(0, 10);
	      gor->setObjectId(pob);
	      std::map<unsigned int, Order*> orlist = myfc->getOrders(gor);
	      //delete gor;
	      for(std::map<unsigned int, Order*>::iterator itcurr = orlist.begin(); itcurr != orlist.end(); ++itcurr){
		std::cout << "Order: slot " << itcurr->second->getSlot() << std::endl;
		std::cout << "type: " << itcurr->second->getOrderType() << std::endl;
		std::cout << "num turns: " << itcurr->second->getNumTurns() << std::endl;
		std::cout << "num params: " << itcurr->second->getNumParameters() << std::endl;

		delete itcurr->second;
	      }

	    }else{
	      //delete order;
	      std::cout << "Failed to replaced order, status " << myfc->getStatus() << std::endl; 
	      status = 10;
	    }

	    // remove order
	    std::cout << "Trying to remove order" << std::endl;

	    RemoveOrder* ro = myfc->createRemoveOrderFrame();
	    ro->setObjectId(pob);
	    ro->removeOrderId(0);
	    if(myfc->removeOrders(ro) == 1){
	      std::cout << "Successfully removed the one order" << std::endl;
	    }else{
	      std::cout << "Failed to remove the one order" << std::endl;
	      status = 9;
	    }
	    delete ro;

	  }else{
	    delete order;
	    std::cout << "Failed to added order, status " << myfc->getStatus() << std::endl;
	    status = 8;
	  }
	  
	  std::cout << "Order test complete, status " << myfc->getStatus() << std::endl;

	  std::cout << "Starting Board test" << std::endl;

	  Board* myboard = myfc->getPersonalBoard();

	  std::cout << "Got personal board, status " << myfc->getStatus() << std::endl;

	  if(myboard != NULL){

	    std::cout << "Personal board, id: " << myboard->getId() << std::endl << "name: " << myboard->getName()
		      << std::endl << "Desc: " << myboard->getDescription() << std::endl << "Num messages: "
		      << myboard->numMessages() << std::endl;

	    std::cout << "Board test finished" << std::endl;

	    std::cout << "Starting Message test, status " << myfc->getStatus() << std::endl;

	    Message* mymess = myfc->createMessageFrame();
	    mymess->setSubject("Test");
	    mymess->setBody("This is a test, pleace check this message is posted");
	    mymess->setBoardId(myboard->getId());
	    mymess->setSlot(0);
	    if(myfc->postMessage(mymess)){
	      std::cout << "Posted message succeessfully, status " << myfc->getStatus() << std::endl;
	    }else{
	      std::cout << "Failed to post message, status " << myfc->getStatus() << std::endl;
	      status = 7;
	    }
	    //delete mymess;

	    std::cout << "Re-fetching board" << std::endl;

	    delete myboard;
	    myboard = myfc->getPersonalBoard();

	    GetMessage* gm = myfc->createGetMessageFrame();
	    gm->setBoard(myboard->getId());
	    gm->addMessageRange(0, myboard->numMessages());
	    std::map<unsigned int, Message*> messages = myfc->getMessages(gm);
	    //delete gm;
	    std::cout << "Downloaded messages" << std::endl;
	    for(std::map<unsigned int, Message*>::iterator itcurr = messages.begin(); itcurr != messages.end();
		++itcurr){
	      Message* mess = (itcurr->second);
	      std::cout << "Message on " << mess->getBoardId() << " in slot " << mess->getSlot() << " with type " << mess->getMessageType() << std::endl;
	      std::cout << "Subject: " << mess->getSubject() << std::endl;
	      std::cout << "Body: " << mess->getBody() << std::endl;
	      delete mess;
	    }

	    std::cout << "Message get and display complete, status " << myfc->getStatus() << std::endl;

	    std::cout << "Starting message delete test" << std::endl;

	    RemoveMessage* rm = myfc->createRemoveMessageFrame();
	    rm->setBoard(myboard->getId());
	    rm->removeMessageId(0); // remove the message we posted
	    if(myfc->removeMessages(rm) == 1){
	      std::cout << "Successfully removed the one message" << std::endl;
	    }else{
	      std::cout << "Failed to remove the one message" << std::endl;
	      status = 6;
	    }
	    //delete rm;

	    std::cout << "Message test complete, status " << myfc->getStatus() << std::endl;

	    delete myboard;

	    std::cout << "Starting Time test" << std::endl;

	    std::cout << "Time remaining: " << myfc->getTimeRemaining() << std::endl;

	    std::cout << "Time test complete, status " << myfc->getStatus() << std::endl;

	    std::cout << "Starting Async frame polling test" << std::endl;

	    myfc->pollForAsyncFrames();

	    std::cout << "Async frame polling test complete, status " << myfc->getStatus() << std::endl;

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
