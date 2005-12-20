
#include "buffer.h"
#include "objectvisitor.h"

#include "planet.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  Planet::Planet(){
  }

  /*! \brief Required virtual destructor.
   */
  Planet::~Planet(){
  }

  /*! \brief Unpack extra data from Buffer.

  First calls Object::unpackBuffer() and then unpacks the owner playerid from
  the Buffer.
  \param buf The Buffer to unpack from.
  \return True if successful.
  */
  bool Planet::unpackBuffer(Buffer* buf){
    Object::unpackBuffer(buf);
    owner = buf->unpackInt();
        for(uint32_t i = buf->unpackInt(); i > 0; i--){
            uint32_t resid = buf->unpackInt();
            uint32_t numsurface = buf->unpackInt();
            uint32_t totalamount = buf->unpackInt();
            buf->unpackInt(); //the amount "unminable"
            resources[resid] = std::pair<uint32_t, uint32_t>(numsurface, totalamount);
        }
    return true;
  }

  /*! \brief Visit this Planet with an ObjectVisitor.
    \param vis The ObjectVisitor that wishes to visit.
  */
  void Planet::visit(ObjectVisitor* vis){
    vis->visit(this);
  }
  
  /*! \brief Gets the playerid of the owner of this Planet.
    \return The playerid of the owner.
  */
  int Planet::getOwner(){
    return owner;
  }

    /*! \brief Get the resources on the planet.
    \return The map of resource ids to amount of surface and total amount pairs.
    */
    std::map<uint32_t, std::pair<uint32_t, uint32_t> > Planet::getResources() const{
        return resources;
    }

}
