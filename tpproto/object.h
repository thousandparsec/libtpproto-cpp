#ifndef TPPROTO_OBJECT_H
#define TPPROTO_OBJECT_H

#include <string>
#include <set>
#include <tpproto/frame.h>
#include <tpproto/vector3d.h>

namespace TPProto{

  class ObjectVisitor;

  class Object : public Frame{
  public:
    virtual ~Object();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

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

    
    

  protected:
    unsigned int id;
    Vector3d pos;
    Vector3d vel;

    unsigned int obtype;
    unsigned long long size;
    std::string name;

    std::set<unsigned int> contained;
    std::set<unsigned int> availableorders;
    int numorders;

  };

}

#endif
