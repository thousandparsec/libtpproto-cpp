

#include "buffer.h"

#include "removeorder.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  RemoveOrder::RemoveOrder(){
  }

  /*! \brief Destructor.
   */
  RemoveOrder::~RemoveOrder(){
    idset.clear();
  }

  /*! \brief Pack this RemoveOrder Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void RemoveOrder::packBuffer(Buffer* buf){
    buf->packInt(object);
    buf->packInt(idset.size());
    for(std::set<unsigned int>::reverse_iterator itcurr = idset.rend(); itcurr != idset.rbegin(); --itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Order_Remove;
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since RemoveOrder frames are only sent to the server, we should not receive 
  them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool RemoveOrder::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Sets the Object id of the Object the Orders are on.
   /param obid The object id number.
  */
  void RemoveOrder::setObjectId(unsigned int obid){
    object = obid;
  }

  /*! \brief Adds a single Order id to remove.
    \param id The order id to remove.
  */
  void RemoveOrder::removeOrderId(unsigned int id){
    idset.insert(id);
  }

  /*! \brief Adds a set of Order ids to the set to remove.
    \param ids The set of order ids to remove.
  */
  void RemoveOrder::removeOrderIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }
  
  /*! \brief Adds a range of Order ids to the set to remove.

  The range is inclusive of the start id, and exclusive of the end.  This
  allows clients to remove all orders by going removeOrderRange(0, 
  object->getNumberOrders()).
    \param start The first order id to remove.
    \param end One greater than the last order id to remove.
  */
  void RemoveOrder::removeOrderRange(unsigned int start, unsigned int end){
    for(unsigned int i = start; i < end; i++){
      idset.insert(i);
    }
  }

}
