#ifndef TPPROTO_OBJECTVISITOR_H
#define TPPROTO_OBJECTVISITOR_H

namespace TPProto{

  class Object;
  
  class ObjectVisitor{
  public:
    virtual ~ObjectVisitor();

    virtual void visit(Object* ob) = 0;
    

  };

}

#endif
