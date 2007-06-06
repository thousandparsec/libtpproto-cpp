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
    /*! \brief The id of the player who "owns" the planet or -1 if not owned or unknown
     */
    int owner;
    /*! \brief A map from resource ids to pairs of surface and minable resources
     */
    std::map<uint32_t, std::pair<uint32_t, uint32_t> > resources;

  };

}

#endif
