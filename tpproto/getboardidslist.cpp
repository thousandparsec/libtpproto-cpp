
#include <cassert>

#include "buffer.h"

#include "getboardidslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetBoardIdsList::GetBoardIdsList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetBoardIdsList::~GetBoardIdsList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetBoardIdsList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetBoardIdsList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft03_BoardIds_Get;
    }


}
