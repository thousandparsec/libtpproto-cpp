#ifndef TPPROTO_FRAMECODEC_H
#define TPPROTO_FRAMECODEC_H

#include <string>
#include <set>
#include <map>

namespace TPProto{

  class TPSocket;
  class Frame;
  class GetObjectByID;
  class GetObjectByPos;
  class GetObjects;
  class Object;
  class Buffer;
  class GetBoard;
  class Board;
  class GetMessage;
  class Message;
  class RemoveMessage;
  class AsyncFrameListener;
  class GetOrder;
  class Order;
  class RemoveOrder;
  class OrderDescription;
  class Logger;


  class FrameCodec{
  public:
    FrameCodec();
    ~FrameCodec();

    //config & setup

    void setClientString(const std::string & name);
    void setSocket(TPSocket * nsock);
    void setAsyncFrameListener(AsyncFrameListener* afl);
    void setLogger(Logger* nlog);

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

    //Orders
    GetOrder* createGetOrderFrame();
    std::map<unsigned int, Order*> getOrders(GetOrder* frame);
    Order* createOrderFrame(int type);
    bool insertOrder(Order* frame);
    bool replaceOrder(Order* frame);
    RemoveOrder* createRemoveOrderFrame();
    int removeOrders(RemoveOrder* frame); // returns number removed
    void seedOrderDescriptionCache(std::set<unsigned int> otypes);

    //Boards and Messages
    GetBoard* createGetBoardFrame();
    std::map<unsigned int, Board*> getBoards(GetBoard* frame);
    Board* getPersonalBoard();
    GetMessage* createGetMessageFrame();
    std::map<unsigned int, Message*> getMessages(GetMessage* frame);
    Message* createMessageFrame();
    bool postMessage(Message* frame);
    RemoveMessage* createRemoveMessageFrame();
    int removeMessages(RemoveMessage* frame); // returns number removed


    //Time
    int getTimeRemaining();

    //polling
    void pollForAsyncFrames();
    
    //send and receive frames
    void sendFrame(Frame * f);
    Frame* recvFrame();

  private:
    Frame* recvOneFrame();
    Object* createObject(Buffer *buf);

    TPSocket * sock;
    AsyncFrameListener* asynclistener;
    Logger* logger;
    int status;
    int version;
    std::string clientid;
    int nextseqnum;

    std::map<unsigned int, OrderDescription*> orderdescCache;

  };

}

#endif
