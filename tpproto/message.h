#ifndef TPPROTO_MESSAGE_H
#define TPPROTO_MESSAGE_H
/*  Message - Frame send by/to the server about a message on a board.
 *
 *  Copyright (C) 2005, 2006, 2008  Lee Begg and the Thousand Parsec Project
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*! \file
  \brief Declares Message Frame.
*/

#include <string>
#include <map>

#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief Enum of known message types, depricated.
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
        uint32_t getTurnNum() const;
        std::map<int32_t, uint32_t> getReferences() const;

    void setBoardId(unsigned int board);
    void setSlot(int nsl);
    void setMessageType(unsigned int mt);
    void setSubject(const std::string &ns);
    void setBody(const std::string &nb);
        void setReferences(std::map<int32_t, uint32_t> rs);

  private:
    unsigned int bid;
    int slot;
    unsigned int mtypes;
    std::string subject;
    std::string body;
        uint32_t turn;
        std::map<int32_t, uint32_t> refs;
  };

}

#endif
