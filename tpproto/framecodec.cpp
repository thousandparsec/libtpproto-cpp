
#include <iostream>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef VERSION
#define VERSION "0.0.0"
#endif

#include "tpsocket.h"
#include "buffer.h"

// Frame Types

#include "okframe.h"
#include "failframe.h"
#include "sequence.h"
#include "connect.h"
#include "login.h"
#include "getobjectbyid.h"
#include "getobjectbypos.h"
#include "getobjects.h"
#include "object.h"
#include "universe.h"
#include "galaxy.h"
#include "starsystem.h"
#include "planet.h"
#include "fleet.h"
#include "getboard.h"
#include "board.h"
#include "getmessage.h"
#include "message.h"
#include "removemessage.h"
#include "gettime.h"
#include "timeremaining.h"
#include "asyncframelistener.h"
#include "order.h"
#include "getorder.h"
#include "removeorder.h"
#include "orderdesc.h"
#include "getorderdesc.h"


#include "framecodec.h"

namespace TPProto {

  FrameCodec::FrameCodec(){
    sock = NULL;
    asynclistener = NULL;
    status = 0;
    version = 2; // TPO2
    clientid = "Unknown client";
    nextseqnum = 1; // should be random
  }

  FrameCodec::~FrameCodec(){
    if(sock != NULL){
      delete sock;
    }
    if(asynclistener != NULL){
      delete asynclistener;
    }

    for(std::map<unsigned int, OrderDescription*>::iterator itcurr = orderdescCache.begin(); 
	itcurr != orderdescCache.end(); ++itcurr){
      delete itcurr->second;
    }
    orderdescCache.clear();
  }

  void FrameCodec::setClientString(const std::string & name){
    clientid = name;
  }

  void FrameCodec::setSocket(TPSocket * nsock){
    if(sock != NULL)
      delete sock;
    sock = nsock;
    status = 0;
  }

  void FrameCodec::setAsyncFrameListener(AsyncFrameListener* afl){
    if(asynclistener != NULL){
      delete asynclistener;
    }
    asynclistener = afl;
  }

  int FrameCodec::getStatus(){
    if(sock == NULL || !sock->isConnected()){
      status = 0;
    }
    return status;
  }
  
  bool FrameCodec::connect(){
    if(sock != NULL || status != 0){
      if(sock->connect()){

	status = 1;

	// send connect frame
	Connect * cf = new Connect();
	cf->setProtocolVersion(version);
	cf->setClientString(std::string("libtpproto-cpp/") + VERSION + " " + clientid);

	sendFrame(cf);
	delete cf;
	
	Frame * reply = recvFrame();

	if(reply != NULL && reply->getType() == ft02_OK){
	  // expect OK back
	  //  or maybe error
	  status = 2;
	  
	  
	  return true;
	}else{
	  status = 0;
	  sock->disconnect();
	}
      }
    }
    return false;
  }
  
  bool FrameCodec::login(const std::string &username, const std::string &password){
    if(status == 2 && sock->isConnected()){
      Login * login = new Login();
      login->setProtocolVersion(version);
      login->setUser(username);
      login->setPass(password);
      sendFrame(login);

      	Frame * reply = recvFrame();

	if(reply != NULL && reply->getType() == ft02_OK){
	  // expect OK back
	  //  or maybe error
	  status = 3;
	  
	  
	  return true;
	}else{
	  //status = 0;
	  //sock->disconnect();
	}

    }
    if(!sock->isConnected())
      status = 0;
    return false;
  }

  void FrameCodec::disconnect(){
    if(status != 0 && sock != NULL)
      sock->disconnect();
    status = 0;
  }

  GetObjectByID* FrameCodec::createGetObjectByIDFrame(){
    GetObjectByID* rtv = new GetObjectByID();
    rtv->setProtocolVersion(version);
    return rtv;
  }
  
  GetObjectByPos* FrameCodec::createGetObjectByPosFrame(){
    GetObjectByPos* rtv = new GetObjectByPos();
    rtv->setProtocolVersion(version);
    return rtv;
  }

  std::map<unsigned int, Object*> FrameCodec::getObjects(GetObjects * frame){
    std::map<unsigned int, Object*> out;
    std::set<unsigned int> ordertypes;
    sendFrame(frame);
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	for(int i = 0; i < ((Sequence*)reply)->getNumber(); i++){
	  Frame * ob = recvFrame();
	  if(ob != NULL && ob->getType() == ft02_Object){
	    out[((Object*)ob)->getId()] = (Object*)ob;
	    if(!((Object*)ob)->getAvailableOrders().empty()){
	      std::set<unsigned int> oldset = ordertypes;
	      ordertypes.clear();
	      std::set<unsigned int> obset = ((Object*)ob)->getAvailableOrders();
	      set_union(oldset.begin(), oldset.end(), obset.begin(), obset.end(), inserter(ordertypes, ordertypes.begin()));
	    }
	  }else{
	    std::cerr << "Expecting object frames, but got " << ob->getType() << " instead" << std::endl;
	  }
	}
      }else if(reply->getType() == ft02_Object){
	out[((Object*)reply)->getId()] = (Object*)reply;
	if(!((Object*)reply)->getAvailableOrders().empty()){
	  std::set<unsigned int> oldset = ordertypes;
	  ordertypes.clear();
	  std::set<unsigned int> obset = ((Object*)reply)->getAvailableOrders();
	  set_union(oldset.begin(), oldset.end(), obset.begin(), obset.end(), inserter(ordertypes, ordertypes.begin()));
	}
      }else{
	//error!
	std::cerr << "Expected object or sequence frame, got " << reply->getType() << std::endl;
      }
    }else{
      std::cerr << "Frame was null, expecting object or sequence" << std::endl;
    }
    if(!ordertypes.empty()){
      seedOrderDescriptionCache(ordertypes);
    }
    return out;
  }

  Object* FrameCodec::getUniverse(){
    GetObjectByID * fr = createGetObjectByIDFrame();
    fr->addObjectID(0);
    sendFrame(fr);
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	// hopefully only one 
	reply = recvFrame();
      }

    }

    return (Object*)reply;
	
  }


  GetOrder* FrameCodec::createGetOrderFrame(){
    GetOrder* f = new GetOrder();
    f->setProtocolVersion(version);
    return f;
  }
  
  std::map<unsigned int, Order*> FrameCodec::getOrders(GetOrder* frame){
    std::map<unsigned int, Order*> out;
    sendFrame(frame);
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	for(int i = 0; i < ((Sequence*)reply)->getNumber(); i++){
	  Frame * ob = recvFrame();
	  if(ob != NULL && ob->getType() == ft02_Order){
	    out[((Order*)ob)->getSlot()] = (Order*)ob;
	  }else if(ob != NULL){
	    std::cerr << "Expecting order frames, but got " << ob->getType() << " instead" << std::endl;
	  }else{
	    std::cerr << "Expecting order frames, but got NULL" << std::endl;
	  
	  }
	}
      }else if(reply->getType() == ft02_Order){
	out[((Order*)reply)->getSlot()] = (Order*)reply;
      }else{
	//error!
	std::cerr << "Expected order or sequence frame, got " << reply->getType() << std::endl;
      }
    }else{
      std::cerr << "Frame was null, expecting order or sequence" << std::endl;
    }
    return out;
  }

  Order* FrameCodec::createOrderFrame(int type){
    Order* f = new Order();
    f->setProtocolVersion(version);
    f->setOrderType(type);
    //get description

    std::map<unsigned int, OrderDescription*>::iterator idesc = orderdescCache.find(type);
    if(idesc != orderdescCache.end()){
      // add parameters
      f->setParameters((idesc->second)->getParameters());

      return f;
    }

    std::cerr << "No order description in cache for type " << type << std::endl;

    delete f;
    return NULL;
  }

  bool FrameCodec::insertOrder(Order* frame){
    sendFrame(frame);
    Frame* reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_OK){
	
	delete reply;
	
	return true;
      }else{
	std::cout << "Expected ok frame, got " << reply->getType() << std::endl;
      }
      delete reply;
    }else{
      std::cout << "Expected ok frame, got NULL" << std::endl;
    }
    return false;
  }

  bool FrameCodec::replaceOrder(Order* frame){
    if(frame->getSlot() >= 0 && insertOrder(frame)){
      
      RemoveOrder* ro =  new RemoveOrder();
      ro->setProtocolVersion(version);
      ro->setObjectId(frame->getObjectId());
      ro->removeOrderId(frame->getSlot() + 1);
      if(removeOrders(ro) == 1){
	delete ro;
	return true;
      }

      delete ro;

    }
    
    return false;

  }

  RemoveOrder* FrameCodec::createRemoveOrderFrame(){
    RemoveOrder* f = new RemoveOrder();
    f->setProtocolVersion(version);
    return f;
  }

  int FrameCodec::removeOrders(RemoveOrder* frame){
    int removed = 0;
    sendFrame(frame);

    Frame* reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	int num = ((Sequence*)reply)->getNumber();
	for(int i = 0; i < num; i++){
	  Frame* f = recvFrame();
	  if(f == NULL)
	    break;
	  if(f->getType() == ft02_OK){
	    removed++;
	  }
	  delete f;
	}
      }else if(reply->getType() == ft02_OK){
	removed++;
      }else{
	std::cout << "Waiting for sequence or ok, got " << reply->getType() << std::endl;
      }
      delete reply;
    }
    return removed;
  }

  void FrameCodec::seedOrderDescriptionCache(std::set<unsigned int> otypes){

    for(std::set<unsigned int>::iterator itcurr = otypes.begin(); 
	itcurr != otypes.end(); ++itcurr){
      if(orderdescCache[*itcurr] != NULL){
	std::set<unsigned int>::iterator itnew = itcurr;
	// this is not ideal...
	if(itcurr == otypes.begin())
	  ++itnew;
	else
	  --itnew;

	otypes.erase(itcurr);
	itcurr = itnew;

      }
    }

    if(!otypes.empty()){
      GetOrderDescription* god = new GetOrderDescription();
      god->setProtocolVersion(version);
      god->addOrderTypes(otypes);
      sendFrame(god);
      delete god;
      
      Frame* reply = recvFrame();
      if(reply != NULL){
	if(reply->getType() == ft02_Sequence){
	  for(int i = 0; i < ((Sequence*)reply)->getNumber(); i++){
	    Frame * ob = recvFrame();
	    
	    if(ob != NULL && ob->getType() == ft02_OrderDesc){
	      OrderDescription* od = (OrderDescription*)ob;
	      orderdescCache[od->getOrderType()] = od;
	    }else{
	      std::cout << "Expected Order Desc frame, got " << reply->getType() << std::endl;
	    }
	  }
	  delete reply;
	}else if(reply->getType() == ft02_OrderDesc){
	  OrderDescription* od = (OrderDescription*)reply;
	  orderdescCache[od->getOrderType()] = od;
	}else{
	  std::cout << "Expected Order Desc frame, got " << reply->getType() << std::endl;
	}
      }else{
	std::cout << "Expected order desc or sequence frame, got NULL" << std::endl;
      }
      
    }
  }
  


  GetBoard* FrameCodec::createGetBoardFrame(){
    GetBoard* f = new GetBoard();
    f->setProtocolVersion(version);
    return f;
  }

  std::map<unsigned int, Board*> FrameCodec::getBoards(GetBoard* frame){
    std::map<unsigned int, Board*> out;
    sendFrame(frame);
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	for(int i = 0; i < ((Sequence*)reply)->getNumber(); i++){
	  Frame * ob = recvFrame();
	  if(ob != NULL && ob->getType() == ft02_Board){
	    out[((Board*)ob)->getId()] = (Board*)ob;
	  }else{
	    std::cerr << "Expecting Board frames, but got " << ob->getType() << " instead" << std::endl;
	  }
	}
      }else if(reply->getType() == ft02_Board){
	out[((Board*)reply)->getId()] = (Board*)reply;
      }else{
	//error!
	std::cerr << "Expected board or sequence frame, got " << reply->getType() << std::endl;
      }
    }else{
      std::cerr << "Frame was null, expecting board or sequence" << std::endl;
    }
    return out;
  }


  Board* FrameCodec::getPersonalBoard(){
    GetBoard * fr = createGetBoardFrame();
    fr->addBoardId(0);
    sendFrame(fr);
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	// hopefully only one 
	reply = recvFrame();
      }
      
    }
    
    return (Board*)reply;
  }

  GetMessage* FrameCodec::createGetMessageFrame(){
    GetMessage* f = new GetMessage();
    f->setProtocolVersion(version);
    return f;
  }

  
  std::map<unsigned int, Message*> FrameCodec::getMessages(GetMessage* frame){
    std::map<unsigned int, Message*> out;
    sendFrame(frame);
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	for(int i = 0; i < ((Sequence*)reply)->getNumber(); i++){
	  Frame * ob = recvFrame();
	  if(ob != NULL && ob->getType() == ft02_Message){
	    out[((Message*)ob)->getSlot()] = (Message*)ob;
	  }else if(ob != NULL){
	    std::cerr << "Expecting message frames, but got " << ob->getType() << " instead" << std::endl;
	  }else{
	    std::cerr << "Expecting message frames, but got NULL"  << std::endl;
	  }
	}
      }else if(reply->getType() == ft02_Message){
	out[((Message*)reply)->getSlot()] = (Message*)reply;
      }else{
	//error!
	std::cerr << "Expected message or sequence frame, got " << reply->getType() << std::endl;
      }
    }else{
      std::cerr << "Frame was null, expecting message or sequence" << std::endl;
    }
    return out;

  }

  Message* FrameCodec::createMessageFrame(){
    Message* f = new Message();
    f->setProtocolVersion(version);
    return f;
  }

  bool FrameCodec::postMessage(Message* frame){
    sendFrame(frame);
    Frame* reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_OK){

	delete reply;

	return true;
      }
      delete reply;
    }
    return false;
  }

  RemoveMessage* FrameCodec::createRemoveMessageFrame(){
    RemoveMessage* f = new RemoveMessage();
    f->setProtocolVersion(version);
    return f;
  }

  int FrameCodec::removeMessages(RemoveMessage* frame){
    int removed = 0;
    sendFrame(frame);

    Frame* reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	int num = ((Sequence*)reply)->getNumber();
	for(int i = 0; i < num; i++){
	  Frame* f = recvFrame();
	  if(f == NULL)
	    break;
	  if(f->getType() == ft02_OK){
	    removed++;
	  }
	  delete f;
	}
      }else if(reply->getType() == ft02_OK){
	removed++;
      }else{
	std::cout << "Waiting for sequence or ok, got " << reply->getType() << std::endl;
      }
      delete reply;
    }
    return removed;
  }

  int FrameCodec::getTimeRemaining(){
    GetTime* gt = new GetTime();
    gt->setProtocolVersion(version);
    sendFrame(gt);
    delete gt;
    Frame* reply = recvFrame();
    if(reply != NULL && reply->getType() == ft02_Time_Remaining){
      int time = ((TimeRemaining*)reply)->getTimeRemaining();
      delete reply;
      return time;
    }
    if(reply != NULL)
      delete reply;
    return -1;
  }


  void FrameCodec::sendFrame(Frame *f){

    Buffer *data = new Buffer();
    f->packBuffer(data);
    Buffer *header = new Buffer();
    header->createHeader(f->getProtocolVersion(), nextseqnum++, f->getType(), data->getLength());

    sock->send(header->getData(), header->getLength(), data->getData(), data->getLength());

    if(nextseqnum == 0)
      nextseqnum++;

  }

  Frame* FrameCodec::recvFrame(){
    char* head, *body;
    int rlen = sock->recvHeader(16, head);
    if(rlen != 16){
      //now what?
      std::cout << "Could not read whole header" << std::endl;
      delete head;
      return NULL;
    }
    
    int len, type, sequ, fver;
    
    Buffer * header = new Buffer();
    header->setData(head, rlen);

    if(!header->readHeader(fver, sequ, type, len)){
      // invalid header
      std::cout << "Header invalid" << std::endl;
      delete header;
      return NULL;
    }

    if(fver != 2){
      std::cout << "Wrong verison of protocol, ver: " << fver << " sequ: " << sequ << " type: " << type << " len: " << len << std::endl;
      delete header;
      sock->disconnect();
      status = 0;
      return NULL;
    }

    rlen = sock->recvBody(len, body);
    if(rlen != len){
      //again, now what?
      std::cout << "Could not read whole body" << std::endl;
      delete header;
      delete body;
      return NULL;
    }

    Buffer *data = new Buffer();
    data->setData(body, rlen);
    
    Frame* frame = NULL;

    // may need to switch on version too
    switch(type){
    case ft02_OK:
      frame = new OKFrame();
      break;

    case ft02_Fail:
      frame = new FailFrame();
      break;

    case ft02_Sequence:
      frame = new Sequence();
      break;

    case ft02_Object:
      frame = createObject(data);
      break;

    case ft02_OrderDesc:
      frame = new OrderDescription();
      break;

    case ft02_Order:
      frame = createOrderFrame(data->peekInt(8));
      break;

    case ft02_Time_Remaining:
      frame = new TimeRemaining();
      break;

    case ft02_Board:
      frame = new Board();
      break;

    case ft02_Message:
      frame = new Message();
      break;

    default:
      //others...
      std::cout << "Received frame of type " << type << " but don't know what to do, setting return value to NULL" << std::endl;
      break;
    }
    
    if(frame != NULL){
      frame->setProtocolVersion(fver);
      if(!frame->unpackBuffer(data)){
	delete frame;
	std::cout << "Unpack Buffer failed" << std::endl;
	frame = NULL;
      }
    }

    if(sequ == 0){
      // async frame, send it on and try again.

      if(asynclistener != NULL){
	if(frame != NULL && frame->getType() == ft02_Time_Remaining){
	  asynclistener->recvTimeRemaining((TimeRemaining*)frame);
	}
      }

      if(frame != NULL){
	delete frame;
      }

      frame = recvFrame();
    }
    
    return frame;

  }

  Object* FrameCodec::createObject(Buffer *buf){
    Object* ob;

    switch(buf->peekInt(4)){
    case 0:
      ob = new Universe();
      break;
    case 1:
      ob = new Galaxy();
      break;
    case 2:
      ob = new StarSystem();
      break;
    case 3:
      ob = new Planet();
      break;
    case 4:
      ob = new Fleet();
      break;

    default:
      ob = NULL;
      break;
    }

    return ob;
  }

  
}// namespace

