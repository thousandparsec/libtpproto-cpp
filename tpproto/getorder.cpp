

#include "buffer.h"

#include "getorder.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  GetOrder::GetOrder(){
  }

  /*! \brief Destructor.
   */
  GetOrder::~GetOrder(){
    idset.clear();
  }

  /*! \brief Pack this GetOrder Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void GetOrder::packBuffer(Buffer* buf){
    buf->packInt(object);
    buf->packInt(idset.size());
    for(std::set<unsigned int>::iterator itcurr = idset.begin(); itcurr != idset.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
    
    type = ft02_Order_Get;
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since GetOrder frames are only sent to the server, we should not receive 
  them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool GetOrder::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Sets the Object id of the Object the Orders are on.
   /param obid The object id number.
  */
  void GetOrder::setObjectId(unsigned int obid){
    object = obid;
  }

  /*! \brief Adds a signle Order id to get.
    \param id The order id to get.
  */
  void GetOrder::addOrderId(unsigned int id){
    idset.insert(id);
  }

  /*! \brief Adds a set of Order ids to the set to request.
    \param ids The set of order ids to get.
  */
  void GetOrder::addOrderIds(std::set<unsigned int> ids){
    for(std::set<unsigned int>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
      idset.insert(*itcurr);
    }
  }

  /*! \brief Adds a range of Order ids to the set to request.

  The range is inclusive of the start id, and exclusive of the end.  This
  allows clients to get all orders by going addOrderRange(0, 
  object->getNumberOrders()).
    \param start The first message id to get.
    \param end One greater than the last message id to get.
  */
  void GetOrder::addOrderRange(unsigned int start, unsigned int end){
    for(unsigned int i = start; i < end; i++){
      idset.insert(i);
    }
  }

}

