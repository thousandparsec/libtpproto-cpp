

#include "buffer.h"

#include "message.h"

namespace TPProto{

  Message::Message(){
  }

  Message::~Message(){
  }

  void Message::packBuffer(Buffer* buf){
    buf->packInt(bid);
    buf->packInt(slot);
    buf->packInt(1);
    buf->packInt(mtypes);
    buf->packString(subject.c_str());
    buf->packString(body.c_str());
    
    type = ft02_Message_Post;
  }

  bool Message::unpackBuffer(Buffer* buf){
    bid = buf->unpackInt();
    slot = buf->unpackInt();
    buf->unpackInt(); // this had better be 1
    mtypes = buf->unpackInt();
    char* temp = buf->unpackString();
    subject = temp;
    delete temp;
    temp = buf->unpackString();
    body = temp;
    delete temp;

    type = ft02_Message;

    return true;
  }

  unsigned int Message::getBoardId(){
    return bid;
  }

  int Message::getSlot(){
    return slot;
  }

  unsigned int Message::getMessageType(){
    return mtypes;
  }

  std::string Message::getSubject(){
    return subject;
  }

  std::string Message::getBody(){
    return body;
  }

  void Message::setBoardId(unsigned int board){
    bid = board;
  }

  void Message::setSlot(int nsl){
    slot = nsl;
  }

  void Message::setMessageType(unsigned int mt){
    mtypes = mt;
  }

  void Message::setSubject(const std::string &ns){
    subject = ns;
  }

  void Message::setBody(const std::string &nb){
    body = nb;
  }

}

