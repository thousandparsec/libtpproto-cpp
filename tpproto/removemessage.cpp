

#include "buffer.h"

#include "removemessage.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  RemoveMessage::RemoveMessage(){
  }

  /*! \brief Destructor.
   */
  RemoveMessage::~RemoveMessage(){
    idset.clear();
  }

  /*! \brief Pack this RemoveMessage Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void RemoveMessage::packBuffer(Buffer* buf){
    buf->packInt(bid);
    buf->packInt(idset.size());
    for(std::set<unsigned int>::reverse_iterator itcurr = idset.rend(); itcurr != idset.rbegin(); --itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Message_Remove;
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since RemoveMessage frames are only sent to the server, we should not receive 
  them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool RemoveMessage::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Sets the Board id of the Board the Messages are on.
   /param board The board id number.
  */
  void RemoveMessage::setBoard(unsigned int board){
    bid = board;
  }

  /*! \brief Adds a single Message id to remove.
    \param id The message id to remove.
  */
  void RemoveMessage::removeMessageId(unsigned int id){
    idset.insert(id);
  }

  /*! \brief Adds a set of Message ids to the set to remove.
    \param ids The set of message ids to remove.
  */
  void RemoveMessage::removeMessageIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

  /*! \brief Adds a range of Message ids to the set to remove.

  The range is inclusive of the start id, and exclusive of the end.  This
  allows clients to remove all messages by going removeMessageRange(0, 
  board->numMessages()).
    \param start The first message id to remove.
    \param end One greater than the last message id to remove.
  */
  void RemoveMessage::removeMessageRange(unsigned int start, unsigned int end){
    for(unsigned int i = start; i < end; i++){
      idset.insert(i);
    }
  }

}
