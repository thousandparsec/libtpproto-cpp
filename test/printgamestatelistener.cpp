
#include <iostream>

#include "printgamestatelistener.h"

using namespace TPProto;

PrintGameStateListener::PrintGameStateListener(){
}

PrintGameStateListener::~PrintGameStateListener(){
}

void PrintGameStateListener::connected(){
    std::cout << "GAMESTATUS: connected" << std::endl;
}

void PrintGameStateListener::redirected(const std::string& url){
    std::cout << "GAMESTATUS: redirected to " << url << std::endl;
}

void PrintGameStateListener::disconnected(){
    std::cout << "GAMESTATUS: disconnected" << std::endl;
}

void PrintGameStateListener::loggedIn(){
    std::cout << "GAMESTATUS: Logged in" << std::endl;
}

void PrintGameStateListener::eotStarted(){
    std::cout << "GAMESTATUS: EOT stated" << std::endl;
}

void PrintGameStateListener::eotEnded(){
    std::cout << "GAMESTATUS: EOT ended" << std::endl;
}

void PrintGameStateListener::timeToEot(uint32_t timetoeot){
    std::cout << "GAMESTATUS: Time to EOT " << timetoeot << std::endl;
}
