#ifndef TPPROTO_GETBOARD_H
#define TPPROTO_GETBOARD_H

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

  class GetBoard : public Frame{
  public:
    GetBoard();
    virtual ~GetBoard();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);
    
    void addBoardId(unsigned int id);
    void addBoardIds(std::set<unsigned int> ids);

  private:
    std::set<unsigned int> idset;

  };

}

#endif
