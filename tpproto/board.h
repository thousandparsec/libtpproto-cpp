#ifndef TPPROTO_BOARD_H
#define TPPROTO_BOARD_H

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

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


  private:
    unsigned int id;
    std::string name;
    std::string desc;
    int nummess;


  };

}

#endif
