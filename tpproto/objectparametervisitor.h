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
  class OrderQueueObjectParam;
  class ResourceListObjectParam;
  class ReferenceObjectParam;
  class RefQuantityListObjectParam;
  class MediaObjectParam;
          
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

    /*! \brief Visit a Velocity 3d object parameter.
    
    All subclasses must override this method.
    \param op The Velocity3dObjectParam to visit.
    */
    virtual void visitObjectParameter(Velocity3dObjectParam* op) = 0;
    
    /*! \brief Visit a Size object parameter.
    
    All subclasses must override this method.
    \param op The SizeObjectParam to visit.
     */
    virtual void visitObjectParameter(SizeObjectParam* op) = 0;
    
    /*! \brief Visit a Integer object parameter.
    
    All subclasses must override this method.
    \param op The IntegerObjectParam to visit.
     */
    virtual void visitObjectParameter(IntegerObjectParam* op) = 0;
    
    /*! \brief Visit a OrderQueue object parameter.
    
    All subclasses must override this method.
    \param op The OrderQueueObjectParam to visit.
     */
    virtual void visitObjectParameter(OrderQueueObjectParam* op) = 0;
    
    /*! \brief Visit a Resource list object parameter.
    
    All subclasses must override this method.
    \param op The ResourceListObjectParam to visit.
     */
    virtual void visitObjectParameter(ResourceListObjectParam* op) = 0;
    
    /*! \brief Visit a Reference object parameter.
    
    All subclasses must override this method.
    \param op The ReferenceObjectParam to visit.
     */
    virtual void visitObjectParameter(ReferenceObjectParam* op) = 0;
    
    /*! \brief Visit a Reference Quality list object parameter.
    
    All subclasses must override this method.
    \param op The RefQuantityListObjectParam to visit.
     */
    virtual void visitObjectParameter(RefQuantityListObjectParam* op) = 0;
    
    /*! \brief Visit a Media object parameter.
    
    All subclasses must override this method.
    \param op The MediaObjectParam to visit.
     */
    virtual void visitObjectParameter(MediaObjectParam* op) = 0;

  };

}

#endif
