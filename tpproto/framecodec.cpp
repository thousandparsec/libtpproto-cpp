
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


#include "framecodec.h"

namespace TPProto {

  FrameCodec::FrameCodec(){
    sock = NULL;
    status = 0;
    version = 2; // TPO2
    clientid = "Unknown client";
    nextseqnum = 1; // should be random
  }

  FrameCodec::~FrameCodec(){
    if(sock != NULL){
      delete sock;
    }
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
    sendFrame(frame);
    Frame * reply = recvFrame();
    if(reply != NULL){
      if(reply->getType() == ft02_Sequence){
	for(int i = 0; i < ((Sequence*)reply)->getNumber(); i++){
	  Frame * ob = recvFrame();
	  if(ob != NULL && ob->getType() == ft02_Object){
	    out[((Object*)ob)->getId()] = (Object*)ob;
	  }else{
	    std::cerr << "Expecting object frames, but got " << ob->getType() << " instead" << std::endl;
	  }
	}
      }else if(reply->getType() == ft02_Object){
	out[((Object*)reply)->getId()] = (Object*)reply;
      }else{
	//error!
	std::cerr << "Expected object or sequence frame, got " << reply->getType() << std::endl;
      }
    }else{
      std::cerr << "Frame was null, expecting object or sequence" << std::endl;
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
      delete head;
      return NULL;
    }
    
    int len, type, sequ, fver;
    
    Buffer * header = new Buffer();
    header->setData(head, rlen);

    if(!header->readHeader(fver, sequ, type, len)){
      // invalid header
      delete header;
      return NULL;
    }

    if(fver != 2){
      
      delete header;
      return NULL;
    }

    rlen = sock->recvBody(len, body);
    if(rlen != len){
      //again, now what?
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

    case ft02_Board:
      frame = new Board();
      break;

    default:
      //others...
      break;
    }
    
    if(frame != NULL && !frame->unpackBuffer(data)){
      delete frame;
      frame = NULL;
    }

    if(sequ == 0){
      // async frame, send it on and try again.

      if(frame){
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

