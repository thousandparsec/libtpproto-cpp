
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
    
    Frame* frame;

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

  
}// namespace

