/*  AccountCreate - frame for creating an account on the server.
 *
 *  Copyright (C) 2006  Lee Begg and the Thousand Parsec Project
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
#ifndef TPPROTO_CREATEACCOUNT_H
#define TPPROTO_CREATEACCOUNT_H

/*! \file
  \brief Declares the AccountCreate Frame.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief The Frame for creating a account on the server.
   */
  class AccountCreate : public Frame{
  public:
    AccountCreate();
    virtual ~AccountCreate();
    
    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setUser(const std::string &name);
    void setPass(const std::string &word);
    void setEmail(const std::string &address);
    void setComment(const std::string &c);

  private:
    std::string user;
    std::string pass;
    std::string email;
    std::string comment;

  };

}

#endif
