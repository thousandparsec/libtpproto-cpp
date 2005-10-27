
#include <cassert>

#include "buffer.h"

#include "getfeatures.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetFeatures::GetFeatures(){
    }

    /*! \brief Required virtual destructor.
    */
    GetFeatures::~GetFeatures(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetFeatures frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetFeatures::packBuffer(Buffer *buf){
            type = ft03_Features_Get;
    }

    /*! \brief Unpack from a Buffer, fails always.
        Since GetFeature frames are only sent to the server, unpacking one doesn't make
        sence, and therefore always returns false.
    \param buf The Buffer to unpack out of, ignored.
    \return False always.
    */
    bool GetFeatures::unpackBuffer(Buffer *buf){
        return false;
    }

}
