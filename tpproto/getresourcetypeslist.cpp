
#include <cassert>

#include "buffer.h"

#include "getresourcetypeslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetResourceTypesList::GetResourceTypesList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetResourceTypesList::~GetResourceTypesList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetResourceTypesList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetResourceTypesList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft03_ResourceTypes_Get;
    }


}
