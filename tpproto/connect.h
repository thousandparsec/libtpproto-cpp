#ifndef TPPROTO_CONNECT_H
#define TPPROTO_CONNECT_H

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

  class Connect : public Frame{
  public:
    Connect();
    virtual ~Connect();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void setClientString(const std::string &cs);

    private:
    std::string clientstring;
    

  };

}

#endif
