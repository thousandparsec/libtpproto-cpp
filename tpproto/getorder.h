#ifndef TPPROTO_GETORDER_H
#define TPPROTO_GETORDER_H

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

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