
#include <cassert>

#include "buffer.h"

#include "getobjectidslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetObjectIdsList::GetObjectIdsList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetObjectIdsList::~GetObjectIdsList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetObjectIdsList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetObjectIdsList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft03_ObjectIds_Get;
    }


}
