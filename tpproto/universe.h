#ifndef TPPROTO_UNIVERSE_H
#define TPPROTO_UNIVERSE_H

#include <tpproto/object.h>

namespace TPProto{

  class Universe : public Object{
  public:
    Universe();
    virtual ~Universe();

    bool unpackBuffer(Buffer* buf);

    void visit(ObjectVisitor* vis);

    int getAge();

  private:
    int age;
  };

}

#endif
