#ifndef TPPROTO_ORDERPARAMETERVISITOR_H
#define TPPROTO_ORDERPARAMETERVISITOR_H

/*! \file
  \brief Declares the OrderParameterVistor interface.
*/

namespace TPProto{

  class SpaceCoordinates;
  class TimeParameter;
  class ObjectParameter;
  class ListParameter;
  class PlayerParameter;
  class SpaceCoordinatesRel;
  class RangeParameter;
  class StringParameter;

  /*! \brief A nice way of handling the various types of OrderParameter classes.

  */
  class OrderParameterVisitor{
  public:
    /*! \brief Required virtual destructor.
     */
    virtual ~OrderParameterVisitor(){};
    
    /*! \brief Visit a SpaceCoordinates order parameter.

    All subclasses must override this method.
    \param op The SpaceCoordinates to visit.
    */
    virtual void visitOrderParam(SpaceCoordinates* op) = 0;

    /*! \brief Visit a TimeParameter order parameter.

    All subclasses must override this method.
    \param op The TimeParameter to visit.
    */
    virtual void visitOrderParam(TimeParameter* op) = 0;

    /*! \brief Visit a ObjectParameter order parameter.

    All subclasses must override this method.
    \param op The ObjectParameter to visit.
    */
    virtual void visitOrderParam(ObjectParameter* op) = 0;

    /*! \brief Visit a ListParameter order parameter.

    All subclasses must override this method.
    \param op The ListParameter to visit.
    */
    virtual void visitOrderParam(ListParameter* op) = 0;

    /*! \brief Visit a PlayerParameter order parameter.

    All subclasses must override this method.
    \param op The PlayerParameter to visit.
    */
    virtual void visitOrderParam(PlayerParameter* op) = 0;

    /*! \brief Visit a SpaceCoordinatesRel order parameter.

    All subclasses must override this method.
    \param op The SpaceCoordinatesRel to visit.
    */
    virtual void visitOrderParam(SpaceCoordinatesRel* op) = 0;

    /*! \brief Visit a RangeParameter order parameter.

    All subclasses must override this method.
    \param op The RangeParameter to visit.
    */
    virtual void visitOrderParam(RangeParameter* op) = 0;

    /*! \brief Visit a StringParameter order parameter.

    All subclasses must override this method.
    \param op The StringParameter to visit.
    */
    virtual void visitOrderParam(StringParameter* op) = 0;

  };

}

#endif
