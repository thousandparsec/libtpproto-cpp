
#include "buffer.h"
#include "objectvisitor.h"

#include "fleet.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  Fleet::Fleet(){
  }

  /*! \brief Destructor.
   */
  Fleet::~Fleet(){
    ships.clear();
  }

  /*! \brief Unpacks the Fleet Object Frame from a Buffer.
    \return True if successful, false otherwise.
   */
  bool Fleet::unpackBuffer(Buffer* buf){
    Object::unpackBuffer(buf);
    owner = buf->unpackInt();
    
    int num = buf->unpackInt();
    for(int i = 0; i < num; i++){
      int stype = buf->unpackInt();
      ships[stype] = buf->unpackInt();
    }

    damage = buf->unpackInt();

    return true;
  }

  /*! \brief Visit the Fleet using an ObjectVisitor.
    
  Passes the Fleet (this) to ObjectVisitor::visit(Fleet*) method.
  \param vis The ObjectVisitor that wishes to visit this Fleet.
  */
  void Fleet::visit(ObjectVisitor* vis){
    vis->visit(this);
  }

  /*! \brief Get the playerid of the owner of the fleet.
    \return The PlayerId of the owner.
   */
  int Fleet::getOwner(){
    return owner;
  }
  
  /*! \brief Get the amount of damage done to the fleet.
    \return The amount of damage.
  */
  int Fleet::getDamage(){
    return damage;
  }

  /*! \brief Get the number of ships of a type that are in the fleet.
    \param stype The type of ship.
    \return The number of ships of the stype type.
  */
  int Fleet::numShips(int stype){
    return ships[stype];
  }


}
