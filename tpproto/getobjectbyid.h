#ifndef TPPROTO_GETOBJECTBYID_H
#define TPPROTO_GETOBJECTBYID_H

#include <tpproto/getobjects.h>
#include <set>

namespace TPProto{
  
  class GetObjectByID : public GetObjects{
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
