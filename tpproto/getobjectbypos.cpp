

#include "buffer.h"

#include "getobjectbypos.h"

namespace TPProto{

  /*! \brief Default constructor.
    
  Sets the position to 0,0,0 (centre of the universe) and the radius to 0.
  */
  GetObjectByPos::GetObjectByPos(){
    pos = Vector3d(0ll, 0ll, 0ll);
    rad = 0ll;
  }

  /*! \brief Required virtual destructor.
   */
  GetObjectByPos::~GetObjectByPos(){

  }
  
  /*! \brief Pack the Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void GetObjectByPos::packBuffer(Buffer* buf){
    pos.pack(buf);
    buf->packInt64(rad);
  }

  /*! \brief Unpack Buffer, always false.
    
  This Frame should never be received, therefore this method always 
  returns false.
  \return False, always.
  */
  bool GetObjectByPos::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Sets the position for the centre of the area to be looked at.
    \param npos The position to be taken as the centre.
  */
  void GetObjectByPos::setPosition(const Vector3d &npos){
    pos = npos;
  }

  /*! \brief Sets the radius for the area to be looked at.
    \param nrad The radius to be used.
  */
  void GetObjectByPos::setRadius(unsigned long long nrad){
    rad = nrad;
  }

}
