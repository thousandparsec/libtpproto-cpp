#ifndef TPPROTO_OBJECTVISITOR_H
#define TPPROTO_OBJECTVISITOR_H

/*! \file
  /brief Declares the ObjectVisitor interface.
*/

namespace TPProto{

  class Object;
  class Universe;
  class Galaxy;
  class StarSystem;
  class Planet;
  class Fleet;
  
  /*! \brief A visitor for visiting Objects.

  Traversing the Object tree is the responsiblity of each
  ObjectVisitor.
  */
  class ObjectVisitor{
  public:
    /*! \brief Required virtual destructor.
     */
    virtual ~ObjectVisitor(){};

    /*! \brief Visit a Universe object.

    All subclasses must override this method.
    \param ob The Universe to visit.
    */
    virtual void visit(Universe* ob) = 0;

    /*! \brief Visit a Galaxy object.

    All subclasses must override this method.
    \param ob The Galaxy to visit.
    */
    virtual void visit(Galaxy* ob) = 0;

    /*! \brief Visit a StarSystem object.

    All subclasses must override this method.
    \param ob The StarSystem to visit.
    */
    virtual void visit(StarSystem* ob) = 0;

    /*! \brief Visit a Planet object.

    All subclasses must override this method.
    \param ob The Planet to visit.
    */
    virtual void visit(Planet* ob) = 0;

    /*! \brief Visit a Fleet object.

    All subclasses must override this method.
    \param ob The Fleet to visit.
    */
    virtual void visit(Fleet* ob) = 0;


    /*! \brief Visit an object.

    All subclasses must override this method.  This method allows new Objects
    without changing this interface and can be used to do general things to
    all objects.
    \param ob The Object to visit.
    */
    virtual void visit(Object* ob) = 0;
    

  };

}

#endif
