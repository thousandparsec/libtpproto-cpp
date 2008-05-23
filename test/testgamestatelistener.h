#ifndef TESTGAMESTATELISTENER_H
#define TESTGAMESTATELISTENER_H
/*  TestGameStateListener - drives Test based on state.
 *
 *  Copyright (C) 2005, 2008  Lee Begg and the Thousand Parsec Project
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
 
#include <tpproto/gamestatuslistener.h>

class NetTest;

class TestGameStateListener : public TPProto::GameStatusListener{
    public:
    TestGameStateListener();
    virtual ~TestGameStateListener();

    virtual void connected();
    virtual bool redirected(const std::string& url);
    virtual void disconnected();
    virtual void loggedIn(bool state);
    virtual void accountCreated(bool state);
    virtual void eotStarted();
    virtual void eotEnded();
    virtual void timeToEot(uint32_t timetoeot);

    NetTest* nettest;
};

#endif
