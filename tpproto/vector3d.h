#ifndef TPPROTO_VECTOR3D_H
#define TPPROTO_VECTOR3D_H

namespace TPProto{
  
  class Buffer;
  
  class Vector3d{
    
  public:
    Vector3d();
    Vector3d(const Vector3d & rhs);
    Vector3d(long long x, long long y, long long z);
    
    Vector3d operator=(const Vector3d & rhs);
    Vector3d operator+(const Vector3d & rhs) const;
    Vector3d operator-(const Vector3d & rhs) const;

    long long getX() const;
    long long getY() const;
    long long getZ() const;
    
    void setAll(long long newx, long long newy, long long newz);
    
    long long getDistance(const Vector3d & origin) const;
    long long getDistanceSq(const Vector3d & origin) const;

    void pack(Buffer * buf) const;
    void unpack(Buffer * buf);

  private:
    long long x;
    long long y;
    long long z;
  
  };

}


#endif
