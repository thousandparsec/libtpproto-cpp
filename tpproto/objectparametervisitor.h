#ifndef TPPROTO_OBJECTPARAMETERVISITOR_H
#define TPPROTO_OBJECTPARAMETERVISITOR_H

/*! \file
  \brief Declares the ObjectParameterVistor interface.
*/

namespace TPProto{

  class Position3dObjectParam;
  class Velocity3dObjectParam;
  class SizeObjectParam;
  class IntegerObjectParam;
          
  /*! \brief A nice way of handling the various types of ObjectParameter classes.

  */
  class ObjectParameterVisitor{
  public:
    /*! \brief Required virtual destructor.
     */
    virtual ~ObjectParameterVisitor(){};
    
    /*! \brief Visit a Position 3d object parameter.

    All subclasses must override this method.
    \param op The Position3dObjectParam to visit.
    */
    virtual void visitObjectParameter(Position3dObjectParam* op) = 0;

    virtual void visitObjectParameter(Velocity3dObjectParam* op) = 0;
    
    virtual void visitObjectParameter(SizeObjectParam* op) = 0;
    
    virtual void visitObjectParameter(IntegerObjectParam* op) = 0;

  };

}

#endif