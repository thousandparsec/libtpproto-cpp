
#include "objectvisitor.h"

#include "starsystem.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  StarSystem::StarSystem(){
  }

  /*! \brief Required virtual destructor.
   */
  StarSystem::~StarSystem(){
  }

  /*! \brief Visit this Starsystem using an ObjectVisitor.
    \param vis The ObjectVisitor that wishes to visit.
  */
  void StarSystem::visit(ObjectVisitor* vis){
    vis->visit(this);
  }

}
