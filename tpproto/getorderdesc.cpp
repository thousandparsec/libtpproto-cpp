
#include "buffer.h"

#include "getorderdesc.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  GetOrderDescription::GetOrderDescription(){
  }

  /*! \brief Destructor.
   */
  GetOrderDescription::~GetOrderDescription(){
    idset.clear();
  }

  /*! \brief Pack this GetOrderDescription Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void GetOrderDescription::packBuffer(Buffer* buf){
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }

    type = ft02_OrderDesc_Get;
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since GetOrderDescription frames are only sent to the server, we should not 
  receive them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool GetOrderDescription::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Adds a signle order type to get.
    \param id The order type to get.
  */
  void GetOrderDescription::addOrderType(unsigned int id){
    idset.insert(id);
  }

  /*! \brief Adds a set of order types to the set to request.
    \param ids The set of order types to get.
  */
  void GetOrderDescription::addOrderTypes(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

}

