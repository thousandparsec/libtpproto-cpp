#ifndef TPPROTO_PLANET_H
#define TPPROTO_PLANET_H

/*! \file
  \brief Declares the Planet Object.
*/

#include <tpproto/object.h>

namespace TPProto{

  /*! \brief A Planet in-game Object.
   */
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
