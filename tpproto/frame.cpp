

#include "buffer.h"

#include "frame.h"

namespace TPProto{

  /*! \brief Standard virtual default destructor.
   */
  Frame::~Frame(){}

  /*! \brief Sets the protocol version number.
    \param ver The version number to use.
  */
  void Frame::setProtocolVersion(int ver){
    protoVer = ver;
  }

  /*! \brief Gets the protocol version number.
    \return The version number.
  */
  int Frame::getProtocolVersion(){
    return protoVer;
  }

  /*! \brief Sets the sequence number to use.
    \param seq The sequence number.
  */
  void Frame::setSequenceNumber(unsigned int seq){
    seqnum = seq;
  }

  /*! \brief Gets the sequence number used.
    \return The sequence number.
  */
  unsigned int Frame::getSequenceNumber(){
    return seqnum;
  }

  /*! \brief Gets the type number for this frame.

  This value should be in the FrameType enum.
  \return The type of this frame.
  */
  int Frame::getType(){
    return type;
  }

}
