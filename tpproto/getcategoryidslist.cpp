
#include <cassert>

#include "buffer.h"

#include "getcategoryidslist.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetCategoryIdsList::GetCategoryIdsList() : GetIdSequence(){
    }

    /*! \brief Required virtual destructor.
    */
    GetCategoryIdsList::~GetCategoryIdsList(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetCategoryIdsList frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetCategoryIdsList::packBuffer(Buffer *buf){
        GetIdSequence::packBuffer(buf);
        type = ft03_CategoryIds_Get;
    }


}
