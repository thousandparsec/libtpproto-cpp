

#include "objectvisitor.h"

#include "galaxy.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  Galaxy::Galaxy(){
  }

  /*! \brief Required virtual destructor.
   */
  Galaxy::~Galaxy(){
  }

  /*! \brief Visit this Galaxy.
    \param vis The ObjectVisitor that wishes to visit.
  */
  void Galaxy::visit(ObjectVisitor* vis){
    vis->visit(this);
  }

}
