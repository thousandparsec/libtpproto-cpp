#ifndef PRINTASYNCLISTENER_H
#define PRINTASYNCLISTENER_H

#include <tpproto/asyncframelistener.h>

class PrintAsyncListener : public TPProto::AsyncFrameListener{
 public:
  PrintAsyncListener();
  virtual ~PrintAsyncListener();

  void recvTimeRemaining(TPProto::TimeRemaining* trf);

};

#endif
