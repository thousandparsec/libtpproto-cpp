

#include "buffer.h"

#include "getobjectbyid.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  GetObjectById::GetObjectById(){
    idset.clear();
  }

  /*! \brief Destructor.
   */
  GetObjectById::~GetObjectById(){
    idset.clear();
  }

  /*! \brief Pack GetObjectById into a Buffer.
    \param buf The buffer to pack into.
  */
  void GetObjectById::packBuffer(Buffer* buf){
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
    type = ft02_Object_GetById;
  }

  /*! \brief Unpack Buffer, always false.
    
  This Frame should never be received, therefore this method always 
  returns false.
  \return False, always.
  */
  bool GetObjectById::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Add a single objectid to get.
    \param id The objectid of the Object to get.
  */
  void GetObjectById::addObjectID(unsigned int id){
    idset.insert(id);
  }

  /*! \brief Add a set of objectids to get.
    \param ids The set of objectids of the Objects to get.
  */
  void GetObjectById::addObjectIDs(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

}
