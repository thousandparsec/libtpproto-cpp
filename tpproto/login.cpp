
#include "buffer.h"

#include "login.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  Login::Login(){
    
  }
  
  /*! \brief Required virtual destructor.
   */
  Login::~Login(){
    
  }
  
  /*! \brief Pack the login into a Buffer.
    \param buf The Buffer to pack into.
  */
  void Login::packBuffer(Buffer *buf){
    buf->packString(user.c_str());
    buf->packString(pass.c_str());
    type = ft02_Login;
  }
  
  /*! \brief Unpack from a Buffer, always false.
    
  Since Login frames are only sent to the server, we should not 
  receive them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool Login::unpackBuffer(Buffer *buf){
    // should never receive a login frame
    return false;
  }
  
  /*! \brief Sets the username to login as.
    \param name The username.
  */
  void Login::setUser(const std::string &name){
    user = name;
  }
  
  /*! \brief Sets the password to login with.
    \param word The password.
  */
  void Login::setPass(const std::string &word){
    pass = word;
  }

}
