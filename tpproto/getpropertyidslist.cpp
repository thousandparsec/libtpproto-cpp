
#include <cassert>

#include "buffer.h"

#include "getpropertyidslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetPropertyIdsList::GetPropertyIdsList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetPropertyIdsList::~GetPropertyIdsList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetPropertyIdsList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetPropertyIdsList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft03_PropertyIds_Get;
    }


}
