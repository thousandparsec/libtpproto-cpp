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

#include <iostream>

#include "nettest.h"

#include "testgamestatelistener.h"

using namespace TPProto;

TestGameStateListener::TestGameStateListener(){
}

TestGameStateListener::~TestGameStateListener(){
}

void TestGameStateListener::connected(){
    std::cout << "GAMESTATUS: connected" << std::endl;
    nettest->login();
}

bool TestGameStateListener::redirected(const std::string& url){
    std::cout << "GAMESTATUS: redirected to " << url << std::endl;
    return true;
}

void TestGameStateListener::disconnected(){
    std::cout << "GAMESTATUS: disconnected" << std::endl;
}

void TestGameStateListener::loggedIn(bool state){
    std::cout << "GAMESTATUS: Logged in" << std::endl;
    if(state){
        nettest->loggedIn();
        nettest->getUniverse();
    }else{
        nettest->stopTest();
    }
}

void TestGameStateListener::accountCreated(bool state){
    std::cout << "GAMESTATUS: account create" << std::endl;
}

void TestGameStateListener::eotStarted(){
    std::cout << "GAMESTATUS: EOT stated" << std::endl;
}

void TestGameStateListener::eotEnded(){
    std::cout << "GAMESTATUS: EOT ended" << std::endl;
}

void TestGameStateListener::timeToEot(uint32_t timetoeot){
    std::cout << "GAMESTATUS: Time to EOT " << timetoeot << std::endl;
}
