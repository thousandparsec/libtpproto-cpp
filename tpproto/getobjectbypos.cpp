

#include "buffer.h"

#include "getobjectbypos.h"

namespace TPProto{

  GetObjectByPos::GetObjectByPos(){
    pos = Vector3d(0ll, 0ll, 0ll);
    rad = 0ll;
  }

  GetObjectByPos::~GetObjectByPos(){

  }
  
  void GetObjectByPos::packBuffer(Buffer* buf){
    pos.pack(buf);
    buf->packInt64(rad);
  }

  bool GetObjectByPos::unpackBuffer(Buffer* buf){
    return false;
  }

  void GetObjectByPos::setPosition(const Vector3d &npos){
    pos = npos;
  }

  void GetObjectByPos::setRadius(unsigned long long nrad){
    rad = nrad;
  }

}
