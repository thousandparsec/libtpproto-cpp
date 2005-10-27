
#include <cassert>

#include "buffer.h"

#include "getordertypeslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetOrderTypesList::GetOrderTypesList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetOrderTypesList::~GetOrderTypesList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetOrderTypesList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetOrderTypesList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft03_OrderTypes_Get;
    }


}
