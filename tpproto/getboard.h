#ifndef TPPROTO_GETBOARD_H
#define TPPROTO_GETBOARD_H

/*! \file
  \brief Declares the GetBoard Frame.
*/

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief A Frame to get Boards from the server.
   */
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
