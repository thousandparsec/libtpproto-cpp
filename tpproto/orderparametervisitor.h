#ifndef TPPROTO_ORDERPARAMETERVISITOR_H
#define TPPROTO_ORDERPARAMETERVISITOR_H

namespace TPProto{

  class SpaceCoordinates;
  class TimeParameter;
  class ObjectParameter;
  class ListParameter;

  class OrderParameterVisitor{
  public:
    virtual ~OrderParameterVisitor(){};

    virtual void visitOrderParam(SpaceCoordinates* op) = 0;
    virtual void visitOrderParam(TimeParameter* op) = 0;
    virtual void visitOrderParam(ObjectParameter* op) = 0;
    virtual void visitOrderParam(ListParameter* op) = 0;

  };

}

#endif
