
#include "buffer.h"

#include "login.h"

namespace TPProto{

  Login::Login(){
    
  }
  
  Login::~Login(){
    
  }
  
  void Login::packBuffer(Buffer *buf){
    buf->packString(user.c_str());
    buf->packString(pass.c_str());
    type = ft02_Login;
  }
  
  bool Login::unpackBuffer(Buffer *buf){
    // should never receive a login frame
    return false;
  }
  
  void Login::setUser(const std::string &name){
    user = name;
  }
  
  void Login::setPass(const std::string &word){
    pass = word;
  }

}
