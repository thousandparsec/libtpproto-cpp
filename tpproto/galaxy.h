#ifndef TPPROTO_GALAXY_H
#define TPPROTO_GALAXY_H

#include <tpproto/object.h>

namespace TPProto{

  class Galaxy : public Object{
  public:
    Galaxy();
    virtual ~Galaxy();

    void visit(ObjectVisitor* vis);

  };

}

#endif
