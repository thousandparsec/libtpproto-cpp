/*  Message - Frame send by/to the server about a message on a board.
 *
 *  Copyright (C) 2005, 2006, 2008  Lee Begg and the Thousand Parsec Project
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

#include "buffer.h"

#include "message.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  Message::Message(){
    mtypes = 0;
  }

  /*! \brief Required virtual destructor.
   */
  Message::~Message(){
  }

  /*! \brief Pack this Message Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void Message::packBuffer(Buffer* buf){
    buf->packInt(bid); 
    buf->packInt(slot);
    if(protoVer == 2){
      buf->packInt(1);
      buf->packInt(mtypes);
    }else{
      buf->packInt(0);
    }
    buf->packString(subject.c_str());
    buf->packString(body.c_str());
    if(protoVer > 2){
      buf->packInt(0); // Turn number, set by server
      buf->packInt(refs.size());
      for(std::map<int32_t, uint32_t>::iterator itcurr = refs.begin(); itcurr != refs.end(); ++itcurr){
        buf->packInt(itcurr->first);
        buf->packInt(itcurr->second);
      }
    }
    
    type = ft02_Message_Post;
  }

  /*! \brief Unpack this Message from a Buffer.
    \param buf The Buffer to unpack out of.
    \return True if successful.
  */
  bool Message::unpackBuffer(Buffer* buf){
    bid = buf->unpackInt();
    slot = buf->unpackInt();
    if(protoVer == 2){
      /* FIXME: Validate */
      buf->unpackInt(); // this had better be 1
      mtypes = buf->unpackInt();
    }else{
      buf->unpackInt(); // Should be 0
    }
    subject = buf->unpackString();
    body = buf->unpackString();
    if(protoVer > 2){
      turn = buf->unpackInt();
      uint32_t numrefs = buf->unpackInt();
      /* Check for DOS */
      if (numrefs > 1000){
        /* FIXME: Cleanup ? */
	return false;
      }
      for(uint32_t i = 0; i < numrefs; i++){
	int32_t refid = buf->unpackInt();
	/* FIXME: Need to validate before just writing to array ! */
	refs[refid] = buf->unpackInt();
      }
    }

    type = ft02_Message;

    return true;
  }

  /*! \brief Gets the Board id number this Message is on.
    \return The board id.
  */
  unsigned int Message::getBoardId(){
    return bid;
  }

  /*! \brief Gets the slot number this Message is in.

  Can be -1 if it is to be the last message.
    \return The slot number.
  */
  int Message::getSlot(){
    return slot;
  }

  /*! \brief Gets the message type.
  \deprecated Not used in TP03 or later. Use references instead.
    \return The message type.
  */
  unsigned int Message::getMessageType(){
    return mtypes;
  }

  /*! \brief Gets the subject of the message.
    \return The subject.
  */
  std::string Message::getSubject(){
    return subject;
  }

  /*! \brief Gets the body text of the message.
    \return The body text.
  */
  std::string Message::getBody(){
    return body;
  }

    /*! \brief Gets the turn number this message was posted on.
    \return The turn number.
    */
    uint32_t Message::getTurnNum() const{
        return turn;
    }

    /*! \brief Gets the references for this message.
    \return The map of references.
    */
    std::map<int32_t, uint32_t> Message::getReferences() const{
        return refs;
    }

  /*! \brief Sets the Board id for the message.
    \param board The Board this message should be posted to.
  */
  void Message::setBoardId(unsigned int board){
    bid = board;
  }

  /*! \brief Sets the slot number for this message to go into.
    
  Starts from 0. Can be -1 for the last slot.
  /param nsl The slot number.
  */
  void Message::setSlot(int nsl){
    slot = nsl;
  }

  /*! \brief Sets the message type.
  \deprecated Not used in TP03 or later. Use references instead.
    \param mt The message type.
  */
  void Message::setMessageType(unsigned int mt){
    mtypes = mt;
  }

  /*! \brief Sets the subject.
    \param ns The subject.
  */
  void Message::setSubject(const std::string &ns){
    subject = ns;
  }

  /*! \brief Sets the body text.
    \param nb The body text.
  */
  void Message::setBody(const std::string &nb){
    body = nb;
  }

    /*! \brief Sets the references for this message
    \param rs The map of references.
    */
    void Message::setReferences(std::map<int32_t, uint32_t> rs){
        refs = rs;
    }

}

