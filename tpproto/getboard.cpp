
#include "buffer.h"

#include "getboard.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  GetBoard::GetBoard(){
  }

  /*! \brief Destructor.
   */
  GetBoard::~GetBoard(){
    idset.clear();
  }

  /*! \brief Pack this GetBoard Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void GetBoard::packBuffer(Buffer* buf){
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Board_Get;
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since GetBoard frames are only sent to the server, we should not receive them
  and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool GetBoard::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Adds a single Board id to get.
    \param id The board id to get.
  */
  void GetBoard::addBoardId(unsigned int id){
    idset.insert(id);
  }

  /*! \brief Adds a set of Board ids to the set to request.
    \param ids The set of board ids to get.
  */
  void GetBoard::addBoardIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

}

