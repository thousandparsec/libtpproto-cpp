#ifndef TPPROTO_REMOVEMESSAGE_H
#define TPPROTO_REMOVEMESSAGE_H

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

  class RemoveMessage : public Frame{
  public:
    RemoveMessage();
    virtual ~RemoveMessage();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setBoard(unsigned int board);
    void removeMessageId(unsigned int id);
    void removeMessageIds(std::set<unsigned int> ids);
    // range is exclusive of end and inclusive of start
    void removeMessageRange(unsigned int start, unsigned int end);

  private:
    int bid;
    std::set<unsigned int> idset;

  };

}

#endif
