#ifndef TPPROTO_BOARD_H
#define TPPROTO_BOARD_H
/*! \file
  \brief Declares Board class.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief Board Frame class.
    
  The Board class is the representation of the board Frame, and provides
  an interface to the boards.
  */
  class Board : public Frame{
  public:

    Board();
    virtual ~Board();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);

    unsigned int getId();
    std::string getName();
    std::string getDescription();

    int numMessages();
    uint64_t getLastModifiedTime();

  private:
    unsigned int id;
    std::string name;
    std::string desc;
    int nummess;
    uint64_t modtime;

  };

}

#endif
