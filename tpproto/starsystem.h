#ifndef TPPROTO_STARSYSTEM_H
#define TPPROTO_STARSYSTEM_H

/*! \file
  \brief Declares StarSystem Object.
*/

#include <tpproto/object.h>

namespace TPProto{

  /*! \brief In-game Object for a StarSystem.
   */
  class StarSystem : public Object{
  public:
    StarSystem();
    virtual ~StarSystem();

    void visit(ObjectVisitor* vis);

  };

}

#endif
