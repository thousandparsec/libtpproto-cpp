
#include <iostream>
#include <tpproto/timeremaining.h>

#include "printasynclistener.h"

using namespace TPProto;

PrintAsyncListener::PrintAsyncListener(){
}

PrintAsyncListener::~PrintAsyncListener(){
}

void PrintAsyncListener::recvTimeRemaining(TimeRemaining* trf){
  std::cout << "ASYNC FRAME: Time Remaining: " << trf->getTimeRemaining() << std::endl;
}
