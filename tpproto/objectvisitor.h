#ifndef TPPROTO_OBJECTVISITOR_H
#define TPPROTO_OBJECTVISITOR_H

namespace TPProto{

  class Object;
  class Universe;
  class Galaxy;
  class StarSystem;
  class Planet;
  class Fleet;
  
  class ObjectVisitor{
  public:
    virtual ~ObjectVisitor(){};

    virtual void visit(Universe* ob) = 0;
    virtual void visit(Galaxy* ob) = 0;
    virtual void visit(StarSystem* ob) = 0;
    virtual void visit(Planet* ob) = 0;
    virtual void visit(Fleet* ob) = 0;

    virtual void visit(Object* ob) = 0;
    

  };

}

#endif
