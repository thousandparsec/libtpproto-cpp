#ifndef PRINTGAMESTATELISTENER_H
#define PRINTGAMESTATELISTENER_H

#include <tpproto/gamestatuslistener.h>

class PrintGameStateListener : public TPProto::GameStatusListener{
    public:
    PrintGameStateListener();
    virtual ~PrintGameStateListener();

    virtual void connected();
    virtual void redirected(const std::string& url);
    virtual void disconnected();
    virtual void loggedIn();
    virtual void eotStarted();
    virtual void eotEnded();
    virtual void timeToEot(uint32_t timetoeot);

};

#endif
