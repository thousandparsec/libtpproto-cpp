
#include <cassert>

#include "buffer.h"

#include "getdesignidslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetDesignIdsList::GetDesignIdsList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetDesignIdsList::~GetDesignIdsList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetDesignIdsList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetDesignIdsList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft03_DesignIds_Get;
    }


}
