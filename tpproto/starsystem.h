#ifndef TPPROTO_STARSYSTEM_H
#define TPPROTO_STARSYSTEM_H

#include <tpproto/object.h>

namespace TPProto{

  class StarSystem : public Object{
  public:
    StarSystem();
    virtual ~StarSystem();

    void visit(ObjectVisitor* vis);

  };

}

#endif
