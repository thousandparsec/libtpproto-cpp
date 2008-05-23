#ifndef TPPROTO_FRAME_H
#define TPPROTO_FRAME_H
/*  Frame - base class for frame objects
 *
 *  Copyright (C) 2005, 2006, 2008  Lee Begg and the Thousand Parsec Project
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



/*! \file
  \brief Declares Frame class, and serveral protocol enums.

  The Frame class is the basic message sent to and from the server.
  The Enums include FrameType and FrameErrorCode.
*/

namespace TPProto{
  
/*! \brief Enum of Frame types.

*/ 
  enum FrameType {
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
    ft02_ResDesc_Get = 22,
    ft02_ResDesc = 23,
        ft03_Redirect = 24,
        ft03_Features_Get = 25,
        ft03_Features = 26,
        ft03_Ping = 27,
        ft03_ObjectIds_Get = 28,
        ft03_ObjectIds_GetByPos = 29,
        ft03_ObjectIds_GetByContainer = 30,
        ft03_ObjectIds = 31,
        ft03_OrderTypes_Get = 32,
        ft03_OrderTypes = 33,
        ft03_Order_Probe = 34,
        ft03_BoardIds_Get = 35,
        ft03_BoardIds = 36,
        ft03_ResourceTypes_Get = 37,
        ft03_ResourceTypes = 38,
        ft03_Player_Get = 39,
        ft03_Player = 40,
        ft03_Category_Get = 41,
        ft03_Category = 42,
        ft03_Category_Add = 43,
        ft03_Category_Remove = 44,
        ft03_CategoryIds_Get = 45,
        ft03_CategoryIds = 46,
        ft03_Design_Get = 47,
        ft03_Design = 48,
        ft03_Design_Add = 49,
        ft03_Design_Modify = 50,
        ft03_Design_Remove = 51,
        ft03_DesignIds_Get = 52,
        ft03_DesignIds = 53,
        ft03_Component_Get = 54,
        ft03_Component = 55,
        ft03_ComponentIds_Get = 56,
        ft03_ComponentIds = 57,
        ft03_Property_Get = 58,
        ft03_Property = 59,
        ft03_PropertyIds_Get = 60,
        ft03_PropertyIds = 61,
        ft03_Account_Create = 62,
        ft04_TurnFinished = 63,
        ft04_Filters_Set = 64,
        ft04_GameInfo_Get = 65,
        ft04_GameInfo = 66,
        ft04_ObjectDesc_Get = 67,
        ft04_ObjectDesc = 68,
        ft04_ObjectTypes_Get = 69,
        ft04_ObjectTypes_List = 70,
        ft04_Object_Modify = 71,
    ft02_Max,
    
    
  };
  
/*! \brief Frame error codes.
  
Used by FailFrame to indicate the type of error.
*/
  enum FrameErrorCode {
    fec_Invalid = -1,
    fec_ProtocolError = 0,
    fec_FrameError = 1,
    fec_PermUnavailable = 2,
    fec_TempUnavailable = 3,
    fec_NonExistant = 4,
    fec_PermissionDenied = 5,
    fec_Max
  };
  
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
