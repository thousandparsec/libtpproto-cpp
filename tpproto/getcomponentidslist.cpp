
#include <cassert>

#include "buffer.h"

#include "getcomponentidslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetComponentIdsList::GetComponentIdsList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetComponentIdsList::~GetComponentIdsList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetComponentIdsList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetComponentIdsList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft03_ComponentIds_Get;
    }


}
