#ifndef TPPROTO_ASYNCFRAMELISTENER_H
#define TPPROTO_ASYNCFRAMELISTENER_H
/*! \file
  \brief Declares the AsyncFrameListener.
*/

namespace TPProto{

  class TimeRemaining;

  /*! \brief Base class for handling asynchronous frames.

  The AsyncFrameListener should be implemented by every app to
  receive frames that do not relate to requests sent by the client.
  Currently there is only one type of asynchronous frame.
  */
  class AsyncFrameListener{
  public:
    /*! Standard virtual destructor */
    virtual ~AsyncFrameListener(){};

    /*! \brief Handles TimeRemaining frames (must be overridden).
      
    This method is used to pass TimeRemaining frames received
    asynchronously to the client application.  recvTimeRemaining
    must be overridden.  The parameter should be deleted when
    no longer needed.

    \param trf The TimeRemaining frame received.
    */
    virtual void recvTimeRemaining(TimeRemaining* trf) = 0;

  };

}

#endif
