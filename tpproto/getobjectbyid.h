#ifndef TPPROTO_GETOBJECTBYID_H
#define TPPROTO_GETOBJECTBYID_H

#include <tpproto/frame.h>
#include <set>

namespace TPProto{
  
  class GetObjectByID : public Frame{
  public:
    GetObjectByID();
    virtual ~GetObjectByID();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void addObjectID(unsigned int id);
    void addObjectIDs(std::set<unsigned int> ids);

  private:
    std::set<unsigned int> idset;
    

  };

}

#endif
