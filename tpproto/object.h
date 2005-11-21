#ifndef TPPROTO_OBJECT_H
#define TPPROTO_OBJECT_H

/*! \file
  \brief Declares Object Frame baseclass.
*/

#include <string>
#include <set>
#include <tpproto/frame.h>
#include <tpproto/vector3d.h>

namespace TPProto{

  class ObjectVisitor;

  /*! \brief The Object Frame baseclass.
    
  All Objects in the game are a subclass of this class.
  */
  class Object : public Frame{
  public:
    virtual ~Object();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    /*! \brief Be visited by an ObjectVisitor.

    This method must be overridden by all subclasses, to call the
    correct visit method in ObjectVisitor.
    \param vis The ObjectVisitor that wants to visit this object.
    */
    virtual void visit(ObjectVisitor * vis) = 0;

    unsigned int getId();
    std::string getName();
    Vector3d getPos();
    Vector3d getVel();
    int getObjectType();
    unsigned long long getSize();
    std::set<unsigned int> getContainedObjectIds();
    std::set<unsigned int> getAvailableOrders();
    int getNumberOrders();
    uint64_t getLastModifiedTime();
    
    

  protected:
    /*! \brief The Object's Id
     */
    unsigned int id;
    
    /*! \brief The current position of the object.
     */
    Vector3d pos;
    
    /*! \brief The current velocity of the object.
     */
    Vector3d vel;

    /*! \brief The object type number.
     */
    unsigned int obtype;
    
    /*! \brief The diameter of the object in units.
     */
    unsigned long long size;
    
    /*! \brief The object's name.
     */
    std::string name;

    /*! \brief The set of objectids that this object contains.
     */
    std::set<unsigned int> contained;

    /*! \brief The set of orderid that can be placed on this object.
     */
    std::set<unsigned int> availableorders;
    
    /*! \brief The number of orders on this object.

    The id of the orders are 0 to numorders-1.
    */
    int numorders;

    /*! \brief The last modification time of this object.
    */
    uint64_t modtime;

  };

}

#endif
