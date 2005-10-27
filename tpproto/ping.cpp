
#include <cassert>

#include "buffer.h"

#include "ping.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    Ping::Ping(){
    }

    /*! \brief Required virtual destructor.
    */
    Ping::~Ping(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this Ping frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void Ping::packBuffer(Buffer *buf){
            type = ft03_Ping;
    }

    /*! \brief Unpack from a Buffer, fails always.
        Since ping frames are only sent to the server, unpacking one doesn't make
        sence, and therefore always returns false.
    \param buf The Buffer to unpack out of, ignored.
    \return False always.
    */
    bool Ping::unpackBuffer(Buffer *buf){
        return false;
    }

}
