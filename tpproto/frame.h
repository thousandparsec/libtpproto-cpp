#ifndef TPPROTO_FRAME_H
#define TPPROTO_FRAME_H

namespace TPProto{
  
  typedef enum {
    // New Frame Codes
    ft02_Invalid = -1,
    ft02_OK = 0,
    ft02_Fail = 1,
    ft02_Sequence = 2,
    ft02_Connect = 3,
    ft02_Login = 4,
    ft02_Object_GetById = 5,
    ft02_Object_GetByPos = 6,
    ft02_Object = 7,
    ft02_OrderDesc_Get = 8,
    ft02_OrderDesc = 9,
    ft02_Order_Get = 10,
    ft02_Order = 11,
    ft02_Order_Insert = 12,
    ft02_Order_Remove = 13,
    ft02_Time_Remaining_Get = 14,
    ft02_Time_Remaining = 15,
    ft02_Board_Get = 16,
    ft02_Board = 17,
    ft02_Message_Get = 18,
    ft02_Message = 19,
    ft02_Message_Post = 20,
    ft02_Message_Remove = 21,
    //only for future reference
    ft02_ResDesc_Get = 22,
    ft02_ResDesc = 23,
    ft02_Max,
    
    
  } FrameType;
  
  typedef enum {
    fec_Invalid = -1,
    fec_ProtocolError = 0,
    fec_FrameError = 1,
    fec_PermUnavailable = 2,
    fec_TempUnavailable = 3,
    fec_NonExistant = 4,
    fec_Max
  } FrameErrorCode;
  
  class Buffer;

  class Frame{
  public:
    virtual ~Frame();

    virtual void packBuffer(Buffer* buf) = 0;
    virtual bool unpackBuffer(Buffer* buf) = 0;

    void setProtocolVersion(int ver);
    int getProtocolVersion();

    int getType();

  protected:
    int protoVer;
    int type;

  private:


  };

}

#endif
