#ifndef TPPROTO_ASYNCFRAMELISTENER_H
#define TPPROTO_ASYNCFRAMELISTENER_H

namespace TPProto{

  class TimeRemaining;

  class AsyncFrameListener{
  public:
    virtual ~AsyncFrameListener(){};

    virtual void recvTimeRemaining(TimeRemaining* trf) = 0;

  };

}

#endif
