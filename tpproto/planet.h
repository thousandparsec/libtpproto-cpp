#ifndef TPPROTO_PLANET_H
#define TPPROTO_PLANET_H

#include <tpproto/object.h>

namespace TPProto{

  class Planet : public Object{
  public:
    Planet();
    virtual ~Planet();

    bool unpackBuffer(Buffer* buf);
    
    void visit(ObjectVisitor* vis);

    int getOwner();

  private:
    int owner;

  };

}

#endif
