#ifndef TPPROTO_GALAXY_H
#define TPPROTO_GALAXY_H

/*! \file
  \brief Declares the Galaxy Object.
*/

#include <tpproto/object.h>

namespace TPProto{

  /*! \brief A Galaxy in-game Object.
   */
  class Galaxy : public Object{
  public:
    Galaxy();
    virtual ~Galaxy();

    void visit(ObjectVisitor* vis);

  };

}

#endif
