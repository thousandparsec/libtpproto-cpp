#ifndef TPPROTO_FLEET_H
#define TPPROTO_FLEET_H

/*! \file
  \brief Declares Fleet Object.
*/

#include <map>

#include <tpproto/object.h>

namespace TPProto{

  /*! \brief Fleet Object.
   */
  class Fleet : public Object{
  public:
    Fleet();
    virtual ~Fleet();

    bool unpackBuffer(Buffer* buf);

    void visit(ObjectVisitor* vis);

    int getOwner();
    unsigned int getDamage();
    int numShips(int stype);
    std::map<int32_t, int32_t> getShips() const;

  private:
    /*! \brief The id of the player who "owns" the fleet or -1 if not owned or unknown
     */
    int owner;
    /*! \brief The amount of damage the fleet currently has
     */
    unsigned int damage;
    /*! \brief A map from ship type to number of ships of this type.
     */
    std::map<int, int> ships;
  };

}

#endif
