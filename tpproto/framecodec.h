#ifndef TPPROTO_FRAMECODEC_H
#define TPPROTO_FRAMECODEC_H

#include <string>


namespace TPProto{

  class TPSocket;
  class Frame;
  class GetObjectByID;
  class GetObjectByPos;

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
    //std::map<unsigned int, Object*> getObjectByID(GetObjectByID * frame);
    GetObjectByPos* createGetObjectByPosFrame();
    //std::map<unsigned int, Object*> getObjectByPos(GetObjectByPos * frame);
    //Object* getUniverse();

    //Order* createOrderFrame(int type);

    
    //send and receive frames
    void sendFrame(Frame * f);
    Frame* recvFrame();

  private:
    TPSocket * sock;
    int status;
    int version;
    std::string clientid;
    int nextseqnum;

  };

}

#endif
