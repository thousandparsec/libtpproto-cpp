#ifndef TPPROTO_FRAMECODEC_H
#define TPPROTO_FRAMECODEC_H

#include <string>
#include <map>

namespace TPProto{

  class TPSocket;
  class Frame;
  class GetObjectByID;
  class GetObjectByPos;
  class GetObjects;
  class Object;
  class Buffer;

  class FrameCodec{
  public:
    FrameCodec();
    ~FrameCodec();

    //config & setup

    void setClientString(const std::string & name);
    void setSocket(TPSocket * nsock);


    //status
    int getStatus();

    //connect, login and disconnect
    bool connect();
    bool login(const std::string &user, const std::string &password);
    void disconnect();
    

    //Objects
    GetObjectByID* createGetObjectByIDFrame();
    GetObjectByPos* createGetObjectByPosFrame();
    std::map<unsigned int, Object*> getObjects(GetObjects * frame);
    Object* getUniverse();

    //Order* createOrderFrame(int type);

    
    //send and receive frames
    void sendFrame(Frame * f);
    Frame* recvFrame();

  private:
    Object* createObject(Buffer *buf);

    TPSocket * sock;
    int status;
    int version;
    std::string clientid;
    int nextseqnum;

  };

}

#endif
