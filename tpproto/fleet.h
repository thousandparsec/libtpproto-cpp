#ifndef TPPROTO_FLEET_H
#define TPPROTO_FLEET_H

#include <map>

#include <tpproto/object.h>

namespace TPProto{

  class Fleet : public Object{
  public:
    Fleet();
    virtual ~Fleet();

    bool unpackBuffer(Buffer* buf);

    void visit(ObjectVisitor* vis);

    int getOwner();
    int getDamage();
    int numShips(int stype);

  private:
    int owner;
    int damage;
    std::map<int, int> ships;
  };

}

#endif
