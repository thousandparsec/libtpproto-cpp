#ifndef TPPROTO_REMOVEORDER_H
#define TPPROTO_REMOVEORDER_H

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

  class RemoveOrder : public Frame{
  public:
    RemoveOrder();
    virtual ~RemoveOrder();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setObjectId(unsigned int obid);
    void removeOrderId(unsigned int id);
    void removeOrderIds(std::set<unsigned int> ids);
    // range is exclusive of end and inclusive of start
    void removeOrderRange(unsigned int start, unsigned int end);

  private:
    int object;
    std::set<unsigned int> idset;

  };

}

#endif
