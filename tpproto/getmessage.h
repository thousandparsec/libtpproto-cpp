#ifndef TPPROTO_GETMESSAGE_H
#define TPPROTO_GETMESSAGE_H

/*! \file
  \brief Declares GetMessage Frame.
*/

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief A Frame to get Messages from one board on the server.
   */
  class GetMessage : public Frame{
  public:
    GetMessage();
    virtual ~GetMessage();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setBoard(unsigned int board);
    void addMessageId(unsigned int id);
    void addMessageIds(std::set<unsigned int> ids);
    // range is exclusive of end and inclusive of start
    void addMessageRange(unsigned int start, unsigned int end);

  private:
    int bid;
    std::set<unsigned int> idset;

  };

}

#endif
