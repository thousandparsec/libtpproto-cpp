#ifndef TPPROTO_PLANET_H
#define TPPROTO_PLANET_H

/*! \file
  \brief Declares the Planet Object.
*/

#include <map>

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
    std::map<uint32_t, std::pair<uint32_t, uint32_t> > getResources() const;

  private:
    int owner;
    std::map<uint32_t, std::pair<uint32_t, uint32_t> > resources;

  };

}

#endif
