#ifndef TPPROTO_GETOBJECTBYID_H
#define TPPROTO_GETOBJECTBYID_H

/*! \file
  \brief Declares GetObjectByID Frame class.
*/

#include <tpproto/getobjects.h>
#include <set>

namespace TPProto{
  
  /*! \brief Frame to send to the server to get one or more
    Object by objectid.
  */
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
