
#include <cassert>

#include "buffer.h"

#include "getidsequence.h"

namespace TPProto{

    /*! \brief Default constructor.
    */
    GetIdSequence::GetIdSequence() : Frame(){
        seqkey = 0xffffffff;
        offset = 0;
        count = 0;
    }

    /*! \brief Required virtual destructor.
    */
    GetIdSequence::~GetIdSequence(){
    }

    /*! \brief Pack into a Buffer.
    
    Packs this GetIdSequence frame into the buffer.
    \param buf Buffer to pack into.
    */ 
    void GetIdSequence::packBuffer(Buffer *buf){
            buf->packInt(seqkey);
            buf->packInt(offset);
            buf->packInt(count);
    }

    /*! \brief Unpack from a Buffer, fails always.
        Since GetIdSequence baseframes are only sent to the server, unpacking one doesn't make
        sence, and therefore always returns false.
    \param buf The Buffer to unpack out of, ignored.
    \return False always.
    */
    bool GetIdSequence::unpackBuffer(Buffer *buf){
        return false;
    }

    /*! \brief Sets the Sequence Key.

    Sets the value of the sequence key to use.  Set to 0xffffffff (-1) to
    get the server to give you a new key.
    \param key The value of the seqkey.
    */
    void GetIdSequence::setSeqKey(uint32_t key){
        seqkey = key;
    }

    /*! \brief Sets the Offset to start at.

        Sets the value of the offset to start the id sequence at.
        \param os The value of the offset.
     */
    void GetIdSequence::setOffset(uint32_t os){
        offset = os;
    }

    /*! \brief Sets the number of ids to get.

        Sets the value of the number of ids to get.
        \param c The value of the number of ids to get.
     */
    void GetIdSequence::setCount(uint32_t c){
        count = c;
    }

    /*! \brief Gets the Sequence Key.

            Gets the value of the sequence key to use.  If is 0xffffffff (-1)
            the server will generate a new key.
        \return The value of the seqkey.
     */
    uint32_t GetIdSequence::getSeqKey() const{
        return seqkey;
    }

    /*! \brief Gets the offset to start the id sequence at.

        Gets the value of the offset the id sequence will be started at.
        \return The value of the offset.
     */
    uint32_t GetIdSequence::getOffset() const{
        return offset;
    }

    /*! \brief Gets the number of ids to get.

        Gets the value of the number of ids this frame will get.
        \return The value of the number of ids to get.
     */
    uint32_t GetIdSequence::getCount() const{
        return count;
    }

}
