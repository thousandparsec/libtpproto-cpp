#ifndef TPPROTO_MESSAGE_H
#define TPPROTO_MESSAGE_H

/*! \file
  \brief Declares Message Frame.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief Enum of known message types.
   */
  enum MessageType{
    mt_Order_Completed = 1,
    mt_Order_Canceled = 2
  };

  /*! \brief A Frame of a Message from a Board.
   */
  class Message : public Frame{
  public:
    Message();
    virtual ~Message();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    unsigned int getBoardId();
    int getSlot();
    unsigned int getMessageType();
    std::string getSubject();
    std::string getBody();

    void setBoardId(unsigned int board);
    void setSlot(int nsl);
    void setMessageType(unsigned int mt);
    void setSubject(const std::string &ns);
    void setBody(const std::string &nb);

  private:
    unsigned int bid;
    int slot;
    unsigned int mtypes;
    std::string subject;
    std::string body;
  };

}

#endif
