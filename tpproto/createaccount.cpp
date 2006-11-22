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
#include "buffer.h"

#include "createaccount.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  AccountCreate::AccountCreate(){
    
  }
  
  /*! \brief Required virtual destructor.
   */
  AccountCreate::~AccountCreate(){
    
  }
  
  /*! \brief Pack the login into a Buffer.
    \param buf The Buffer to pack into.
  */
  void AccountCreate::packBuffer(Buffer *buf){
    buf->packString(user.c_str());
    buf->packString(pass.c_str());
    buf->packString(email.c_str());
    buf->packString(comment.c_str());
    type = ft02_Account_Create;
  }
  
  /*! \brief Unpack from a Buffer, always false.
    
  Since AccountCreate frames are only sent to the server, we should not 
  receive them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool AccountCreate::unpackBuffer(Buffer *buf){
    // should never receive a login frame
    return false;
  }
  
  /*! \brief Sets the username to create the account.
    \param name The username.
  */
  void AccountCreate::setUser(const std::string &name){
    user = name;
  }
  
  /*! \brief Sets the password to create the account with.
    \param word The password.
  */
  void AccountCreate::setPass(const std::string &word){
    pass = word;
  }
  
  /*! \brief Sets the email address to create the account with.
    \param address The email address.
  */
  void AccountCreate::setEmail(const std::string &address){
    email = address;
  }
  
  /*! \brief Sets the comment to create the account with.
    \param c The comment.
  */
  void AccountCreate::setComment(const std::string &c){
    comment = c;
  }

}
