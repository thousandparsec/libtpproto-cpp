#ifndef TPPROTO_LOGIN_H
#define TPPROTO_LOGIN_H

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

  class Login : public Frame{
  public:
    Login();
    virtual ~Login();
    
    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setUser(const std::string &name);
    void setPass(const std::string &word);

  private:
    std::string user;
    std::string pass;

  };

}

#endif
