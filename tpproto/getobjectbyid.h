#ifndef TPPROTO_GETOBJECTBYID_H
#define TPPROTO_GETOBJECTBYID_H

/*! \file
  \brief Declares GetObjectById Frame class.
*/

#include <tpproto/getobjects.h>
#include <set>

namespace TPProto{
  
  /*! \brief Frame to send to the server to get one or more
    Object by objectid.
  */
    class GetObjectById : public GetObjects{
  public:
        GetObjectById();
        virtual ~GetObjectById();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void addObjectID(unsigned int id);
    void addObjectIDs(std::set<unsigned int> ids);

        protected:
        /*! \brief The set of object ids of the objects to get.
        */
    std::set<unsigned int> idset;
    

  };

}

#endif
