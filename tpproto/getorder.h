#ifndef TPPROTO_GETORDER_H
#define TPPROTO_GETORDER_H

/*! \file
  \brief Declares GetOrder Frame.
*/

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief A Frame to get Orders from an Object on the server.
   */
  class GetOrder : public Frame{
  public:
    GetOrder();
    virtual ~GetOrder();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);
    
    void setObjectId(unsigned int obid);
    void addOrderId(unsigned int id);
    void addOrderIds(std::set<unsigned int> ids);
    void addOrderRange(unsigned int start, unsigned int end); //[start... end)

  private:
    std::set<unsigned int> idset;
    int object;
  };

}

#endif
