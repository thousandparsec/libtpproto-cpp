/*  Does a network test of the library.
 *
 *  Copyright (C) 2004, 2005, 2008  Lee Begg and the Thousand Parsec Project
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <iostream>
#include <stdarg.h>
#include <boost/bind.hpp>

#include <tpproto/object.h>
#include <tpproto/board.h>
#include <tpproto/message.h>
#include <tpproto/order.h>
#include <tpproto/player.h>
#include <tpproto/timeparameter.h>
#include <tpproto/logger.h>
#include <tpproto/gamelayer.h>
#include <tpproto/simpleeventloop.h>

#include <tpproto/objectcache.h>
#include <tpproto/boardcache.h>
#include <tpproto/playercache.h>

//#include "downloadprintvisitor.h"
#include "testgamestatelistener.h"

#include "nettest.h"

using namespace TPProto;

class StdoutLogger : public Logger{
    public:
        StdoutLogger(){}
        virtual ~StdoutLogger(){}

        void error(const char* mesg, ...){
            printf("[error] ");
            va_list ap;
            va_start(ap, mesg);
            vprintf(mesg, ap);
            va_end(ap);
            printf("\n");
        }
        void warning(const char* mesg, ...){
            printf("[warning] ");
            va_list ap;
            va_start(ap, mesg);
            vprintf(mesg, ap);
            va_end(ap);
            printf("\n");
        }
        void info(const char* mesg, ...){
            printf("[info] ");
            va_list ap;
            va_start(ap, mesg);
            vprintf(mesg, ap);
            va_end(ap);
            printf("\n");
        }
        void debug(const char* mesg, ...){
            printf("[debug] ");
            va_list ap;
            va_start(ap, mesg);
            vprintf(mesg, ap);
            va_end(ap);
            printf("\n");
        }

};

int main(int argc, char** argv){
  
  std::cout << "TPProto library test (network)" << std::endl;
  
  if(argc < 2){
    
    std::cerr << "Need at least one argument" << std::endl;
    std::cerr << "nettest <tp server url> [<username> <password>]" << std::endl;

    return 1;
  }else{
      NetTest nettest;
      
      nettest.setUrl(argv[1]);
      
      if(argc == 4){
          nettest.setUserPass(argv[2], argv[3]);
      }
      nettest.doTest();
      
      return nettest.getStatus();
  }
}

NetTest::NetTest(){
    status = 0;
    mygame = new GameLayer();
    myel = new SimpleEventLoop();
    mygame->setEventLoop(myel);
    mygame->setLogger(new StdoutLogger());
    TestGameStateListener * testlistener = new TestGameStateListener();
    testlistener->nettest = this;
    mygame->setGameStatusListener(testlistener);
    mygame->setClientString("libtpproto-cpp_NetTest");
    
    username = "test";
    password = "test";

    std::cout << "Test setup complete" << std::endl;

    std::cout << "Starting status: " << mygame->getStatus() << std::endl;
    
}

NetTest::~NetTest(){
    std::cout << "Cleaning up after tests" << std::endl;
    delete mygame;
    delete myel;
    std::cout << "Clean up done, exiting" << std::endl;
}

int NetTest::getStatus(){
    return status;
}

void NetTest::setUrl(const std::string& nu){
    url = nu;
}

void NetTest::setUserPass(const std::string& nu, const std::string& np){
    username = nu;
    password = np;
}

void NetTest::doTest(){
    myel->setTimer(120, boost::bind(&NetTest::stopTest, this));
    
    myel->setTimer(2, boost::bind(&NetTest::connect, this));
    
    myel->runEventLoop();
}

void NetTest::stopTest(){
    if(mygame->getStatus() != gsDisconnected){
        mygame->disconnect();
    }
    myel->endEventLoop();
}

void NetTest::allDone(){
    status = 0;
    stopTest();
}

void NetTest::connect(){
    status = 2;
    if(mygame->connect(url)){
      std::cout << "Connected ok, status: " << mygame->getStatus() << std::endl;
    }else{
        std::cout << "Could not start connection: " << mygame->getStatus() << std::endl;
        stopTest();
    }
}

void NetTest::login(){
    status = 3;
        if(mygame->login(username, password)){
            std::cout << "Login ok, status: " << mygame->getStatus() << std::endl;
        }else{
            std::cout << "Could not start login: " << mygame->getStatus() << std::endl;
            stopTest();
        }
}

void NetTest::getUniverse(){
    status = 4;
    mygame->getObjectCache()->requestObject(0, boost::bind(&NetTest::receiveUniverse, this, _1));
}

void NetTest::getAllObjectIds(){
    status = 5;
    mygame->getObjectCache()->requestObjectIds(boost::bind(&NetTest::receiveAllObjectIds, this, _1));
}

void NetTest::getBoard(){
    status = 6;
    mygame->getBoardCache()->requestBoard(0, boost::bind(&NetTest::receiveBoard, this, _1));
}

void NetTest::getPlayer(){
    status = 7;
    mygame->getPlayerCache()->requestPlayer(1, boost::bind(&NetTest::receivePlayer, this, _1));
}


void NetTest::receiveUniverse(boost::shared_ptr<Object> universe){
    if(universe){
        std::cout << "Got the Universe object ok, status: " << mygame->getStatus() << std::endl;
        std::cout << "ID: " << universe->getId() << std::endl;
        std::cout << "name: " << universe->getName() << std::endl;
        std::cout << "type: " << universe->getObjectType() << std::endl;
        std::cout << "size: " << universe->getSize() << std::endl;
        //std::cout << "age: " << universe->getAge() << std::endl;

        std::set<unsigned int> obset = universe->getContainedObjectIds();
        std::cout << "Num contained: " << obset.size() << std::endl << "Contains: ";
        for(std::set<unsigned int>::iterator itcurr = obset.begin(); itcurr != obset.end(); ++itcurr){
            std::cout << (*itcurr) << " ";
        }
        std::cout << std::endl;
        
        getAllObjectIds();
    }else{
        std::cerr << "Did not get the universe, status: " << mygame->getStatus() << std::endl;
        stopTest();
    }
}

void NetTest::receiveAllObjectIds(std::set<uint32_t> ids){
    std::cout << "Received All Object Ids" << std::endl;
    
    getBoard();
}

void NetTest::receiveBoard(boost::shared_ptr<Board> board){
    if(board){
        std::cout << "Personal board, id: " << board->getId() << std::endl << "name: " << board->getName()
                << std::endl << "Desc: " << board->getDescription() << std::endl << "Num messages: "
                << board->numMessages() << std::endl;
        getPlayer();
    }else{
        std::cerr << "Did not get Board, status: " << mygame->getStatus() << std::endl;
        stopTest();
    }
}

void NetTest::receivePlayer(boost::shared_ptr<Player> player){
    if(player){
        std::cout << "Got player" << std::endl;
        std::cout << "Playerid: " << player->getPlayerId() << std::endl;
        std::cout << "Name: " << player->getPlayerName() << std::endl;
        std::cout << "Race: " << player->getRaceName() << std::endl;
        allDone();
    }else{
        std::cerr << "Did not get Player, status: " << mygame->getStatus() << std::endl;
        stopTest();
    }
    
}


// 	Object* universe = myfc->getUniverse();
// 	if(universe != NULL){
// 	  
// 	  std::cout << "Got the Universe object ok, status: " << myfc->getStatus() << std::endl;
// 
// 	  std::cout << "Setting up Visitor test" << std::endl;
// 	  DownloadPrintVisitor* dpv = new DownloadPrintVisitor();
//             dpv->setGameLayer(myfc);
// 
// 	  
// 	  std::cout << "Starting Visitor test" << std::endl;
// 	  universe->visit(dpv);
// 
// 	  delete universe;
// 
// 	  int pob = dpv->getPlayableObject();
// 	  delete dpv;
// 
// 	  std::cout << "Finished Visitor test, status: " << myfc->getStatus() << std::endl;
// 	  
// 	  std::cout << "Starting Order test" << std::endl;
// 	  
// 	  Order* order = myfc->createOrderFrame(0);
// 	  order->setObjectId(pob);
// 	  order->setSlot(0);
// 	  if(myfc->insertOrder(order)){
// 	    std::cout << "Successfully added order, status " << myfc->getStatus() << std::endl;
// 
// 	    Order* order = myfc->getOrder(pob, 0);
// 
// 	    std::cout << "Order: slot " << order->getSlot() << std::endl;
// 	    std::cout << "obid: " << order->getObjectId() << std::endl;
// 	    std::cout << "type: " << order->getOrderType() << std::endl;
// 	    std::cout << "num turns: " << order->getNumTurns() << std::endl;
// 	    std::cout << "num params: " << order->getNumParameters() << std::endl;
// 
// 	    //mod order
// 	    TimeParameter* timep = (TimeParameter*)order->getParameter(0);
// 	    timep->setTurns(10);
// 	    
// 	    if(myfc->replaceOrder(order)){
// 	      std::cout << "Successfully replaced order, status " << myfc->getStatus() << std::endl;
// 
// 	      std::list<Order*> orlist = myfc->getOrders(pob, 10);
// 	      for(std::list<Order*>::iterator itcurr = orlist.begin(); itcurr != orlist.end(); ++itcurr){
// 		std::cout << "Order: slot " << (*itcurr)->getSlot() << std::endl;
// 		std::cout << "type: " << (*itcurr)->getOrderType() << std::endl;
// 		std::cout << "num turns: " << (*itcurr)->getNumTurns() << std::endl;
// 		std::cout << "num params: " << (*itcurr)->getNumParameters() << std::endl;
// 
// 		delete *itcurr;
// 	      }
// 
// 	    }else{
// 	      //delete order;
// 	      std::cout << "Failed to replaced order, status " << myfc->getStatus() << std::endl; 
// 	      status = 10;
// 	    }
// 
// 	    // remove order
// 	    std::cout << "Trying to remove order" << std::endl;
// 
// 	    if(myfc->removeOrder(pob, 0)){
// 	      std::cout << "Successfully removed the one order" << std::endl;
// 	    }else{
// 	      std::cout << "Failed to remove the one order" << std::endl;
// 	      status = 9;
// 	    }
// 
// 	  }else{
// 	    delete order;
// 	    std::cout << "Failed to added order, status " << myfc->getStatus() << std::endl;
// 	    status = 8;
// 	  }
// 	  
// 	  std::cout << "Order test complete, status " << myfc->getStatus() << std::endl;
// 
// 	  std::cout << "Starting Board test" << std::endl;
// 
// 	  Board* myboard = myfc->getPersonalBoard();
// 
// 	  std::cout << "Got personal board, status " << myfc->getStatus() << std::endl;
// 
// 	  if(myboard != NULL){
// 
// 	    std::cout << "Personal board, id: " << myboard->getId() << std::endl << "name: " << myboard->getName()
// 		      << std::endl << "Desc: " << myboard->getDescription() << std::endl << "Num messages: "
// 		      << myboard->numMessages() << std::endl;
// 
// 	    std::cout << "Board test finished" << std::endl;
// 
// 	    std::cout << "Starting Message test, status " << myfc->getStatus() << std::endl;
// 
// 	    Message* mymess = myfc->createMessage();
// 	    mymess->setSubject("Test");
// 	    mymess->setBody("This is a test, pleace check this message is posted");
// 	    mymess->setBoardId(myboard->getId());
// 	    mymess->setSlot(0);
// 	    if(myfc->postMessage(mymess)){
// 	      std::cout << "Posted message succeessfully, status " << myfc->getStatus() << std::endl;
// 	    }else{
// 	      std::cout << "Failed to post message, status " << myfc->getStatus() << std::endl;
// 	      status = 7;
// 	    }
// 	    //delete mymess;
// 
// 	    std::cout << "Re-fetching board" << std::endl;
// 
// 	    delete myboard;
// 	    myboard = myfc->getPersonalBoard();
// 
// 	    std::list<Message*> messages = myfc->getMessages(myboard->getId(), myboard->numMessages());
// 
// 	    std::cout << "Downloaded messages" << std::endl;
// 	    for(std::list<Message*>::iterator itcurr = messages.begin(); itcurr != messages.end();
// 		++itcurr){
// 	      Message* mess = *itcurr;
// 	      std::cout << "Message on " << mess->getBoardId() << " in slot " << mess->getSlot() << " with type " << mess->getMessageType() << std::endl;
// 	      std::cout << "Subject: " << mess->getSubject() << std::endl;
// 	      std::cout << "Body: " << mess->getBody() << std::endl;
// 	      delete mess;
// 	    }
// 
// 	    std::cout << "Message get and display complete, status " << myfc->getStatus() << std::endl;
// 
// 	    std::cout << "Starting message delete test" << std::endl;
// 
// 
// 	    if(myfc->removeMessage(myboard->getId(), 0)){ // remove the message we posted
// 	      std::cout << "Successfully removed the one message" << std::endl;
// 	    }else{
// 	      std::cout << "Failed to remove the one message" << std::endl;
// 	      status = 6;
// 	    }
// 
// 	    std::cout << "Message test complete, status " << myfc->getStatus() << std::endl;
// 
// 	    delete myboard;
// 
// 	    std::cout << "Starting Time test" << std::endl;
// 
// 	    std::cout << "Time remaining: " << myfc->getTimeRemaining() << std::endl;
// 
// 	    std::cout << "Time test complete, status " << myfc->getStatus() << std::endl;
// 
// 	  }else{
// 	    std::cout << "Board test failed, board is NULL, status " << myfc->getStatus() << std::endl;
// 	    status = 5;
// 	  }
// 
// 	}else{
// 	  std::cout << "Failed to get Universe Object, status" << myfc->getStatus() << std::endl;
// 	  status = 4;
// 	}
// 	
//       }else{
// 	std::cout << "Failed to login, status: " << myfc->getStatus() << std::endl;
// 	status = 2;
//       }
//       
//       myfc->disconnect();
//       if(myfc->getStatus() != 0){
// 	status = 3;
//       }
//       
//       std::cout << "Disconnected, status: " << myfc->getStatus() << std::endl;
//       
//     }else{
//       std::cout << "Failed to Connect, status: " << myfc->getStatus() << std::endl;
//       status = 1;
//     }
//     
//     std::cout << "Cleaning up after tests" << std::endl;
//     delete myfc;
//     std::cout << "Clean up done, exiting" << std::endl;
//     
//     
//   }
//   
//   return status;
// }
