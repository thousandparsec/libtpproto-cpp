#ifndef TPPROTO_FRAME_H
#define TPPROTO_FRAME_H

/*! \file
  \brief Declares Frame class, and serveral protocol enums.

  The Frame class is the basic message sent to and from the server.
  The Enums include FrameType and FrameErrorCode.
*/

namespace TPProto{
  
/*! \brief Enum of Frame types.

  Just version 02.
*/ 
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
  
/*! \brief Frame error codes.
  
Used by FailFrame to indicate the type of error.
*/
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

/*! \brief Frame baseclass.

Frame is the basic message sent to and from the server.  It stores the current
protocol version number, the sequence number of the frame and the type.
*/
  class Frame{
  public:
    virtual ~Frame();

    /*! \brief Virtual method for packing this Frame into a Buffer.

    This method must be overridden by all subclasses.  Some subclasses may
    make a call to assert(0) if they should not be sent to the server.
    \param buf The Buffer to pack into.
    */
    virtual void packBuffer(Buffer* buf) = 0;

    /*! \brief Virtual method for unpacking this Frame from a Buffer.
      
    This method must be overridden by all subclasses.  Some subclasses may
    always return false if they should never be received from the server.
    \param buf The Buffer to unpack out of.
    \return True if successfully unpacked, false otherwise.
    */
    virtual bool unpackBuffer(Buffer* buf) = 0;

    void setProtocolVersion(int ver);
    int getProtocolVersion();

    void setSequenceNumber(unsigned int seq);
    unsigned int getSequenceNumber();

    int getType();

  protected:
    /*! \brief The protocol version.
     */
    int protoVer;
    /*! \brief The sequence number.
     */
    int seqnum;
    /*! \brief The type of this Frame.

    Should always be in the FrameType enum.
    */
    int type;

  private:


  };

}

#endif
