
#include <cassert>

#include "buffer.h"

#include "getobjecttypeslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetObjectTypesList::GetObjectTypesList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetObjectTypesList::~GetObjectTypesList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetObjectTypesList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetObjectTypesList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft04_ObjectTypes_Get;
    }


}
