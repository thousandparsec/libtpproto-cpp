/*  Vector3d class
 *
 *  Copyright (C) 2005, 2008  Lee Begg and the Thousand Parsec Project
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <math.h>

#include "buffer.h"

#include "vector3d.h"

namespace TPProto{
  
  /*! \brief Default constructor.
   */
  Vector3d::Vector3d(){
    
  }
  
  /*! \brief Constructs a new Vector3d from 3 int64_ts.
    \param newx The x value.
    \param newy The y value.
    \param newz The z value.
   */
  Vector3d::Vector3d(int64_t newx, int64_t newy, int64_t newz){
    x = newx;
    y = newy;
    z = newz;
  }

  /*! \brief Copy constructor.
    \param rhs The Vector3d to copy.
   */
  Vector3d::Vector3d(const Vector3d & rhs){
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
  }
  
  /*! \brief Assignment operator.

  Sets this vectors values to the right hand side's.
  \param rhs The Vector3d to take the value of.
  \return This vector (allows a = b = c).
  */
  Vector3d Vector3d::operator=(const Vector3d & rhs){
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    
    return *this;
  }
  
  /*! \brief Plus operator.

  Adds this vector to the right hand side and returns a new vector
  as the result.
  a = b + c
  \param rhs The Vector3d on the right hand side of the plus sign.
  \return The Vector3d object with sum of this vector and the rhs.
  */
  Vector3d Vector3d::operator+(const Vector3d & rhs) const{
    Vector3d rtn;
    rtn.x = x + rhs.x;
    rtn.y = y + rhs.y;
    rtn.z = z + rhs.z;
    return rtn;
  }
  
  /*! \brief Subtract operator.
    
  Subtracts the right had side from this vector and returns a vector
  with the value.
  a = b - c.
  \param rhs The right hand side of the subtract sign.
  \return The Vector3d with the difference.
  */
  Vector3d Vector3d::operator-(const Vector3d & rhs) const{
    Vector3d rtn;
    rtn.x = x - rhs.x;
    rtn.y = y - rhs.y;
    rtn.z = z - rhs.z;
    return rtn;
  }
  
  /*! \brief Gets the X value of the vector.
    \return The x value.
  */
  int64_t Vector3d::getX() const{
    return x;
  }
  
  /*! \brief Gets the Y value of the vector.
    \return The y value.
  */
  int64_t Vector3d::getY() const{
    return y;
  }

  /*! \brief Gets the Y value of the vector.
    \return The y value.
  */
  int64_t Vector3d::getZ() const{
    return z;
  }
  
  /*! \brief Sets all three conponents of the vector.
    \param newx The new x value.
    \param newy The new y value.
    \param newz The new z value.
  */
  void Vector3d::setAll(int64_t newx, int64_t newy, int64_t newz){
    x = newx;
    y = newy;
    z = newz;
  }
  
  /*! \brief Gets the distance between this point and another vector.

  Uses getDistanceSq().
    \param origin The other vector.
    \return The distance between the vectors.
  */
  int64_t Vector3d::getDistance(const Vector3d & origin) const{
    return (int64_t)sqrt(getDistanceSq(origin));
  }

  /*! \brief Gets the square of the distance between this vector and 
    another vector.

    Same as getDistance() to the power of two.  Faster and useful sometimes.
    \param origin The other vector.
    \return The square of the distatnce between the vectors.
  */
  uint64_t Vector3d::getDistanceSq(const Vector3d & origin) const{
    int64_t dx = x - origin.x;
    int64_t dy = y - origin.y;
    int64_t dz = z - origin.z;
    return (uint64_t)(dx * dx) + (uint64_t)(dy * dy) + (uint64_t)(dz * dz);
  }

  /*! \brief Pack the vector into a Buffer.
    \param buf The Buffer to pack into.
  */
  void Vector3d::pack(Buffer * buf) const{
    buf->packInt64(x);
    buf->packInt64(y);
    buf->packInt64(z);
  }
  
  /*! \brief Unpack the vector from a Buffer.
    \param buf The Buffer to unpack out of.
  */
  void Vector3d::unpack(Buffer * buf){
    x = buf->unpackInt64();
    y = buf->unpackInt64();
    z = buf->unpackInt64();
  }

}
