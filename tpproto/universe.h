#ifndef TPPROTO_UNIVERSE_H
#define TPPROTO_UNIVERSE_H

/*! \file
  \brief Declares the Universe Object.
*/

#include <tpproto/object.h>

namespace TPProto{

  /*! \brief The Universe ingame Object.
   */
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
