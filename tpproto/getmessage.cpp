

#include "buffer.h"

#include "getmessage.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  GetMessage::GetMessage(){
  }

  /*! \brief Destructor.
   */
  GetMessage::~GetMessage(){
    idset.clear();
  }

  /*! \brief Pack this GetMessage Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void GetMessage::packBuffer(Buffer* buf){
    buf->packInt(bid);
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Message_Get;
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since GetMessage frames are only sent to the server, we should not receive 
  them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool GetMessage::unpackBuffer(Buffer* buf){
    return false;
  }  

  /*! \brief Sets the Board id of the Board the Messages are on.
   /param board The board id number.
  */
  void GetMessage::setBoard(unsigned int board){
    bid = board;
  }

  /*! \brief Adds a single Message id to get.
    \param id The message id to get.
  */
  void GetMessage::addMessageId(unsigned int id){
    idset.insert(id);
  }

  /*! \brief Adds a set of Message ids to the set to request.
    \param ids The set of message ids to get.
  */
  void GetMessage::addMessageIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

  /*! \brief Adds a range of Message ids to the set to request.

  The range is inclusive of the start id, and exclusive of the end.  This
  allows clients to get all messages by going addMessageRange(0, 
  board->numMessages()).
    \param start The first message id to get.
    \param end One greater than the last message id to get.
  */
  void GetMessage::addMessageRange(unsigned int start, unsigned int end){
    for(unsigned int i = start; i < end; i++){
      idset.insert(i);
    }
  }

}

