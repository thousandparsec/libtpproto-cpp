
#include <cassert>

#include "buffer.h"

#include "board.h"

namespace TPProto{

  /*! \brief Default Constructor.
   */
  Board::Board(){
  }

  /*! \brief Virtual destructor as required.
   */
  Board::~Board(){
  }

  /*! \brief Packs the board into a buffer.

      Currently assets(0).  Should not be used, sending boards to the 
      server is not needed at this time.
      \param buf The buffer to pack into.
  */
  void Board::packBuffer(Buffer *buf){
    assert(0);
  }

  /*! \brief Unpacks a board from a buffer.

    Unpacks a buffer, setting the values given by the server.
    \param buf The buffer to unpack from.
    \return True if successful.
  */
  bool Board::unpackBuffer(Buffer* buf){
    id = buf->unpackInt();
    char* temp = buf->unpackString();
    name = temp;
    delete[] temp;
    temp = buf->unpackString();
    desc = temp;
    delete[] temp;

    nummess = buf->unpackInt();

    type = ft02_Board;

    return true;
  }

  /*! \brief Gets the board's id number.
      \return The Board ID number.
  */
  unsigned int Board::getId(){
    return id;
  }

  /*! \brief Gets the board's name.
      \return The board's name.
  */
  std::string Board::getName(){
    return name;
  }

  /*! \brief Gets the board's description.
    \return The board's description.
  */
  std::string Board::getDescription(){
    return desc;
  }

  /*! \brief Get the number of messages on the board.

      The id of the messages are 0 to numMessages() - 1.
      \return The number of messages.
  */
  int Board::numMessages(){
    return nummess;
  }
  
}
