


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
#include "logger.h"
#include "silentlogger.h"


#include "framecodec.h"

namespace TPProto {

  /*! \brief Constructs object and sets up defaults.

  Defaults are
    - no TPSocket
    - no AsyncFrameListener
    - SilentLogger for the Logger
    - version 2 of the protocol
    - "Unknown client" for the client string
  */
  FrameCodec::FrameCodec(){
    sock = NULL;
    asynclistener = NULL;
    logger = new SilentLogger();
    status = 0;
    version = 2; // TPO2
    clientid = "Unknown client";
    nextseqnum = 1; // should be random
  }

  /*! \brief Destructor.
   */
  FrameCodec::~FrameCodec(){
    if(sock != NULL){
      delete sock;
    }
    if(asynclistener != NULL){
      delete asynclistener;
    }
    delete logger;

    for(std::map<unsigned int, OrderDescription*>::iterator itcurr = orderdescCache.begin(); 
	itcurr != orderdescCache.end(); ++itcurr){
      delete itcurr->second;
    }
    orderdescCache.clear();
  }

  /*! \brief Sets the client string.

  The client string can be set to anything.  The perferred format is
  "name/version".  The library name and version is added the the client
  string later.
  \param name The client name string.
  */
  void FrameCodec::setClientString(const std::string & name){
    clientid = name;
  }

  /*! \brief Sets the TPSocket to be used for communicating with the server.

  \param nsock The TPSocket to be used.
  */
  void FrameCodec::setSocket(TPSocket * nsock){
    if(sock != NULL)
      delete sock;
    sock = nsock;
    status = 0;
  }

  /*! \brief Sets the AsyncFrameListener.

  Only one AsyncFrameListener is allowed currently.  When a new listener
  is set, the old one if present is deleted.
  \param afl The AsyncFrameListener to use.
  */
  void FrameCodec::setAsyncFrameListener(AsyncFrameListener* afl){
    if(asynclistener != NULL){
      delete asynclistener;
    }
    asynclistener = afl;
  }

  /*! \brief Sets the Logger.

  This method sets a new Logger.  The old Logger is deleted.  If the pointer
  to the new Logger is NULL, the default SilentLogger is used.
  /param nlog The new Logger to use, or NULL
  */  
  void FrameCodec::setLogger(Logger* nlog){
    delete logger;
    logger = nlog;
    if(logger == NULL){
      logger = new SilentLogger();
    }
    logger->debug("Logger set");
  }

  /*! \brief Gets the status of the connection.
    \return The status (int).
  */
  int FrameCodec::getStatus(){
    if(sock == NULL || !sock->isConnected()){
      status = 0;
    }
    return status;
  }
  
  /*! \brief Connects to the server.
  
  Establishes a connection to the server via the TPSocket, sends the Connect
  Frame and waits for the OkFrame to signal it has connected successfully.
  \return True if connected, false otherwise.
  */
  bool FrameCodec::connect(){
    if(sock != NULL || status != 0){
      if(sock->connect()){
	logger->debug("Connection opened");
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
	  logger->info("Connected");
	  delete reply;
	  return true;
	}else{
	  status = 0;
	  logger->error("Could not connect");
	  sock->disconnect();
	  if(reply != NULL)
	    delete reply;
	}
      }else{
	logger->error("Could not open socket to server");
      }
    }else{
      logger->warning("Already connected or no socket, ignoring connect attempt");
    }
    return false;
  }
  
  /*! \brief Logs in to the server.

  Sends a Login Frame to the server and waits for a reply.
  \param username The username to connect as.
  \param password The password of the account of the username.
  \return True if successful, false otherwise.
  */
  bool FrameCodec::login(const std::string &username, const std::string &password){
    if(status == 2 && sock->isConnected()){
      Login * login = new Login();
      login->setProtocolVersion(version);
      login->setUser(username);
      login->setPass(password);
      sendFrame(login);
      delete login;

      	Frame * reply = recvFrame();

	if(reply != NULL && reply->getType() == ft02_OK){
	  // expect OK back
	  //  or maybe error
	  status = 3;
	  logger->info("Logged in");
	  delete reply;
	  return true;
	}else{
	  logger->warning("Did not log in");
	  //status = 0;
	  //sock->disconnect();
	  if(reply != NULL)
	    delete reply;
	}

    }
    if(!sock->isConnected())
      status = 0;
    return false;
  }

  /*! \brief Disconnects from server.

  Closes the underlying TPSocket.
  */
  void FrameCodec::disconnect(){
    if(status != 0 && sock != NULL){
      sock->disconnect();
      logger->info("Disconnected");
    }
    status = 0;
  }

  /*! \brief Creates an GetObjectByID Frame.

  Creates the GetObjectByID object and sets the protocol version.
  \returns A new GetObjectByID object.
  */
  GetObjectByID* FrameCodec::createGetObjectByIDFrame(){
    GetObjectByID* rtv = new GetObjectByID();
    rtv->setProtocolVersion(version);
    return rtv;
  }

  /*! \brief Creates an GetObjectByPos Frame.

  Creates the GetObjectByPos object and sets the protocol version.
  \returns A new GetObjectByPos object.
  */
  GetObjectByPos* FrameCodec::createGetObjectByPosFrame(){
    GetObjectByPos* rtv = new GetObjectByPos();
    rtv->setProtocolVersion(version);
    return rtv;
  }

  /*! \brief Gets objects from the server.
    
  Sends the given GetObjects Frame (either GetObjectByID or GetObjectByPos 
  Frame) and returns the Objects.
  \param frame The GetObjects Frame to request some objects.
  \return A map of object id and Object pairs.
  */
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
	    logger->debug("Expecting object frames, but got %d instead", ob->getType());
	  }
	}
	delete reply;
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
	logger->debug("Expected object or sequence frame, got %d", reply->getType());
	delete reply;
      }
    }else{
      logger->debug("Frame was null, expecting object or sequence");
    }
    if(!ordertypes.empty()){
      seedOrderDescriptionCache(ordertypes);
    }
    return out;
  }

  /*! \brief Gets the Universe Object.
    
  A handy method to get the Universe Object.
  \return The Object of the Universe.
  */
  Object* FrameCodec::getUniverse(){
    GetObjectByID * fr = createGetObjectByIDFrame();
    fr->addObjectID(0);
    sendFrame(fr);
    delete fr;
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	// hopefully only one 
	delete reply;
	reply = recvFrame();
      }

    }

    return (Object*)reply;
	
  }

  /*! \brief Creates a GetOrder Frame.

  This methods creates an GetOrder Frame and sets the correct
  version number.
  \return A new GetOrder frame.
  */
  GetOrder* FrameCodec::createGetOrderFrame(){
    GetOrder* f = new GetOrder();
    f->setProtocolVersion(version);
    return f;
  }
  
  /*! \brief Gets Orders from the server.

  This method sends the GetOrder Frame to the server and returns the
  Order Frames.
  \param frame The GetOrder Frame to send.
  \return Map of OrderId and Order pairs.
  */
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
	      logger->debug("Expecting order frames, but got %d instead", ob->getType());
	      delete ob;
	  }else{
	    logger->debug("Expecting order frames, but got NULL");
	  
	  }
	}
	delete reply;
      }else if(reply->getType() == ft02_Order){
	out[((Order*)reply)->getSlot()] = (Order*)reply;
      }else{
	//error!
	logger->debug("Expected order or sequence frame, got %d", reply->getType());
	delete reply;
      }
    }else{
      logger->debug("Frame was null, expecting order or sequence");
    }
    return out;
  }

  /*! \brief Creates an Order Frame of a given type.

  This method creates a new Order Frame, sets the protocol version and
  sets up the order for the given type, including parameters.
  \param type The type number for the order type.
  \return The new Order.
  */
  Order* FrameCodec::createOrderFrame(int type){
    Order* f = new Order();
    f->setProtocolVersion(version);
    //get description

    std::map<unsigned int, OrderDescription*>::iterator idesc = orderdescCache.find(type);
    if(idesc != orderdescCache.end()){
      // add parameters
      f->setOrderType((idesc->second));

      return f;
    }

    logger->debug("No order description in cache for type %d", type);

    delete f;
    return NULL;
  }

  /*! \brief Inserts an Order into the objects order queue.

  \param frame The Order to insert.
  \returns True if successful, false otherwise.
  */
  bool FrameCodec::insertOrder(Order* frame){
    sendFrame(frame);
    Frame* reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_OK){
	
	delete reply;
	
	return true;
      }else{
	logger->debug("Expected ok frame, got %d", reply->getType());
      }
      delete reply;
    }else{
      logger->debug("Expected ok frame, got NULL");
    }
    return false;
  }

  /*! \brief Replaces a current Order with a new one.

  First inserts the new Order, then removes the old one.
  Can fail with or without the new order in the order queue.
  \param frame The Order that will replace the current one.
  \return True if successful, false otherwise.
  */
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

  /*! \brief Creates a RemoveOrder Frame.
    
  Sets the protocol version as well.
  \return A new RemoveOrder frame.
  */
  RemoveOrder* FrameCodec::createRemoveOrderFrame(){
    RemoveOrder* f = new RemoveOrder();
    f->setProtocolVersion(version);
    return f;
  }

  /*! \brief Removes Orders from the server.
    
  Sends the RemoveOrder frame to the server and receives reply.
  \param frame The RemoveOrder frame to send.
  \return The number of Orders removed.
  */
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
	logger->debug("Waiting for sequence or ok, got %d", reply->getType());
      }
      delete reply;
    }
    return removed;
  }

  /*! \brief Fetches the OrderDescription for the given OrderTypes.
    
  /param otypes Set of order types to get for the cache.
  */
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
	      logger->debug("Expected Order Desc frame, got %d", reply->getType());
	    }
	  }
	  delete reply;
	}else if(reply->getType() == ft02_OrderDesc){
	  OrderDescription* od = (OrderDescription*)reply;
	  orderdescCache[od->getOrderType()] = od;
	}else{
	  logger->debug("Expected Order Desc frame, got %d", reply->getType());
	}
      }else{
	logger->debug("Expected order desc or sequence frame, got NULL");
      }
      
    }
  }
  

  /*! \brief Creates a GetBoard Frame.

  Also sets the protocol version number.
  \return A new GetBoard Frame.
  */
  GetBoard* FrameCodec::createGetBoardFrame(){
    GetBoard* f = new GetBoard();
    f->setProtocolVersion(version);
    return f;
  }

  /*! \brief Gets Boards from the server.

  Sends the GetBoard Frame and gets the Boards back from the server.
  \param frame The GetBoard frame to send to the server.
  \return A map of BoardId and Board pairs.
  */
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
	    logger->debug("Expecting Board frames, but got %d instead", ob->getType());
	  }
	}
      }else if(reply->getType() == ft02_Board){
	out[((Board*)reply)->getId()] = (Board*)reply;
      }else{
	//error!
	logger->debug("Expected board or sequence frame, got %d", reply->getType());
      }
    }else{
      logger->debug("Frame was null, expecting board or sequence");;
    }
    return out;
  }

  /*! \brief Gets the logged in player's personal Board.
    
  A little easier and quicker than FrameCodec::getBoards.
  \return The Board object for the Player's Board.
  */
  Board* FrameCodec::getPersonalBoard(){
    GetBoard * fr = createGetBoardFrame();
    fr->addBoardId(0);
    sendFrame(fr);
    delete fr;
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	// hopefully only one 
	reply = recvFrame();
      }
      
    }
    
    return (Board*)reply;
  }

  /*! \brief Creates a GetMessage Frame.

  Also sets the protocol version.
  \return A new GetMessage frame.
  */
  GetMessage* FrameCodec::createGetMessageFrame(){
    GetMessage* f = new GetMessage();
    f->setProtocolVersion(version);
    return f;
  }

  /*! \brief Gets Messages from the server.

  Sends the GetMessage Frame and receives the Message frames.
  \param frame The GetMessage frame to send.
  \return Map of MessageId and Message pairs.
  */
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
	    logger->debug("Expecting message frames, but got %d instead", ob->getType());
	  }else{
	    logger->debug("Expecting message frames, but got NULL");
	  }
	}
	delete reply;
      }else if(reply->getType() == ft02_Message){
	out[((Message*)reply)->getSlot()] = (Message*)reply;
      }else{
	//error!
	logger->debug("Expected message or sequence frame, got %d", reply->getType());
	delete reply;
      }
    }else{
      logger->debug("Frame was null, expecting message or sequence");
    }
    return out;

  }

  /*! \brief Creates a Message frame.

  Also sets the protocol version.
  \return A new Message object.
  */
  Message* FrameCodec::createMessageFrame(){
    Message* f = new Message();
    f->setProtocolVersion(version);
    return f;
  }

  /*! \brief Posts a Message to the server.
    
  Sends the Message Frame to the server.
  \param frame The Message to post.
  \return True if successful, false otherwise.
  */
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

  /*! \brief Creates a RemoveMessage frame.

  Also sets the version number.
  \returns A new RemoveMessage frame.
  */
  RemoveMessage* FrameCodec::createRemoveMessageFrame(){
    RemoveMessage* f = new RemoveMessage();
    f->setProtocolVersion(version);
    return f;
  }

  /*! \brief Removes messages from the server.

  Sends the RemoveMessage frame and receives the replies.
  \param frame The RemoveMessage frame to send.
  \return The number of Messages removed.
  */
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
	logger->debug("Waiting for sequence or ok, got %d", reply->getType());
      }
      delete reply;
    }
    return removed;
  }

  /*! \brief Gets the time remaining before the end of turn.

  Fetches the time remaining till the end of turn from the server.
  \returns The time in seconds before the end of turn, or
  -1 if there was an error.
  */
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

  /*! \brief Checks the connection for any AsyncFrames.

  This should be called occasionally when there hasn't been any
  interaction between the client and the server.  Most of the time,
  asynchronous frames will be picked up while processing other frames,
  but in times of inactivity it a good idea to poll this.  The acturacy
  of data such as any asynchronous TimeRemaining frame will on how close to
  the time the frame was received to when this method (or any method that
  receives any frame from the server) is called.
  */
  void FrameCodec::pollForAsyncFrames(){
    if(status == 3){
      if(sock->poll()){
	Frame* frame = recvOneFrame();
	if(frame != NULL){
	  if(frame->getSequenceNumber() == 0){
	    // async frame, send it on and try again.
	    
	    if(asynclistener != NULL){
	      if(frame->getType() == ft02_Time_Remaining){
		asynclistener->recvTimeRemaining((TimeRemaining*)frame);
	      }else{
		logger->warning("Unknown Async Frame type, ignoring");
	      }
	    }else{
	      logger->warning("No AsyncFrameListener, discarding frame");
	    }
	  }else{
	    logger->error("Was expecting Async frame but received a frame of type %d with sequence number %d",
			  frame->getType(), frame->getSequenceNumber());
	  }

	  delete frame;
	}else{
	  logger->debug("Frame was null, should have been an async frame");
	}
      }

    }else{
      logger->warning("Trying to poll for async frames but not connected or logged in");
    }
  }

  /*! \brief Sends a Frame.

  Packs the Frame into a Buffer and sends it via the TPSocket.  Sets the 
  sequence number and increments the sequence number counter.
  \param f The Frame to send.
  */
  void FrameCodec::sendFrame(Frame *f){
    if(status >= 1){
      Buffer *data = new Buffer();
      f->packBuffer(data);
      Buffer *header = new Buffer();
      header->createHeader(f->getProtocolVersion(), nextseqnum++, f->getType(), data->getLength());
      
      sock->send(header->getData(), header->getLength(), data->getData(), data->getLength());
      
      delete data;
      delete header;

      if(nextseqnum == 0)
	nextseqnum++;
      
    }
  }

  /*! \brief Receives a normal Frame.

  Receives a normal Frame from the TPSocket, passing any asynchronous frames
  to the AsyncFrameListener until a normal frame is received.
  \return The received Frame or NULL if no frame is received.
  */
  Frame* FrameCodec::recvFrame(){
    Frame* frame = recvOneFrame();
    
    if(frame != NULL && frame->getSequenceNumber() == 0){
      // async frame, send it on and try again.
      
      if(asynclistener != NULL){
	if(frame->getType() == ft02_Time_Remaining){
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

  /*! \brief Receives one Frame from the network.

  Grabs one Frame from the TPSocket.
  \return The received Frame or NULL.
  */
  Frame* FrameCodec::recvOneFrame(){
    if(status >= 1){
      char* head, *body;
      int rlen = sock->recvHeader(16, head);
      if(rlen != 16){
	//now what?
	logger->warning("Could not read whole header");
	delete head;
	return NULL;
      }
      
      int len, type, sequ, fver;
      
      Buffer * header = new Buffer();
      header->setData(head, rlen);
 
      if(!header->readHeader(fver, sequ, type, len)){
	// invalid header
	logger->warning("Header invalid");
	delete header;
	return NULL;
      }
      delete header;
      
      if(fver != 2){
	logger->warning("Wrong verison of protocol, ver: %d sequ: %d type: %d len: %d", fver, sequ, type, len);
	sock->disconnect();
	status = 0;
	return NULL;
      }
      
      rlen = sock->recvBody(len, body);
      if(rlen != len){
	//again, now what?
	logger->warning("Could not read whole body");
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
	logger->warning("Received frame of type %d but don't know what to do, setting return value to NULL", type);
	break;
      }
      
      if(frame != NULL){
	frame->setProtocolVersion(fver);
	frame->setSequenceNumber(sequ);
	if(!frame->unpackBuffer(data)){
	  delete frame;
	  logger->error("Unpack Buffer failed");
	  frame = NULL;
	}
      }
      
      delete data;

      return frame;
    }
    return NULL;
  }

  /*! \brief Creates the correct Object object from the Buffer.

  Looks at the contents of the buffer in order to create the correct 
  Object based on the object type number.
  \param buf The Buffer to create the Object from.
  \return The created Object.
  */
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

