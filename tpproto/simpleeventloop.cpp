/*  SimpleEventLoop - an example simple event loop.
 *
 *  Copyright (C) 2008  Lee Begg and the Thousand Parsec Project
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

#include <sys/select.h>
#include <sys/types.h>
#include <iostream>

#include "tpsocket.h"

#include "simpleeventloop.h"

namespace TPProto{
    
    bool Timer::operator>(const Timer& rhs) const{
        return expiretime > rhs.expiretime;
    }

    SimpleEventLoop::SimpleEventLoop() : running(false), readset(), writeset(), timers(){
    }
    
    SimpleEventLoop::~SimpleEventLoop(){
        for(std::set<TPSocket*>::iterator itcurr = readset.begin(); itcurr != readset.end();
                ++itcurr){
            delete (*itcurr);
        }
        readset.clear();
        writeset.clear();
    }

    void SimpleEventLoop::listenForSocketRead(TPSocket* sock){
        readset.insert(sock);
    }
    
    void SimpleEventLoop::listenForSocketWrite(TPSocket* sock){
        writeset.insert(sock);
    }
    
    TimerConnection SimpleEventLoop::setTimer(uint32_t interval, const TimerSignal::slot_type& callback){
        Timer timer;
        timer.expiretime = time(NULL) + interval;
        timer.signal = new TimerSignal();
        TimerConnection tc = timer.signal->connect(callback);
        timers.push(timer);
        return tc;
    }
            
    void SimpleEventLoop::runEventLoop(){
        running =true;
        
        struct timeval tv;
        
        while(running){
            
            while(!timers.empty() && timers.top().expiretime <= time(NULL)){
                Timer timer = timers.top();
                timers.pop();
                (*timer.signal)();
                delete timer.signal;
            }
            
            if(!running)
                break;
            
            if(timers.empty()){
                tv.tv_sec = 120;
                tv.tv_usec = 0;
            }else{
                tv.tv_sec = (timers.top().expiretime - time(NULL)) - 1;
                if(tv.tv_sec <= 0){
                    tv.tv_sec = 0;
                    tv.tv_usec = 200000;
                }else{
                    tv.tv_usec = 0;
                }
            }
            
            // create fd sets
            fd_set read_set;
            FD_ZERO(&read_set);
            
            int maxfd = 0;
            
            for(std::set<TPSocket*>::iterator itcurr = readset.begin(); itcurr != readset.end();
                    ++itcurr){
                int fd = (*itcurr)->getFileDescriptor();
                FD_SET(fd, &read_set);
                if(maxfd < fd){
                    maxfd = fd;
                }
            }
            
            fd_set write_set;
            FD_ZERO(&write_set);
            
            for(std::set<TPSocket*>::iterator itcurr = writeset.begin(); itcurr != writeset.end();
                    ++itcurr){
                int fd = (*itcurr)->getFileDescriptor();
                FD_SET(fd, &write_set);
                if(maxfd < fd){
                    maxfd = fd;
                }
            }
            
            if(select(maxfd+1, &read_set, &write_set, NULL, &tv) > 0){
                
                for(std::set<TPSocket*>::iterator itcurr =writeset.begin(); itcurr != writeset.end();
                        ++itcurr){
                    if(FD_ISSET((*itcurr)->getFileDescriptor(), &write_set)){
                        TPSocket* sock = *itcurr;
                        writeset.erase(itcurr);
                        try{
                            sock->readyToSend();
                            
                        }catch(DisconnectedException* e){
                            writeset.erase(sock);
                            readset.erase(sock);
                        }
                        // modifed set, itcurr invalid, just wait for the select loop to go around again
                        break;
                    }
                }
                
                for(std::set<TPSocket*>::iterator itcurr = readset.begin(); itcurr != readset.end();
                        ++itcurr){
                    if(FD_ISSET((*itcurr)->getFileDescriptor(), &read_set)){
                        try{
                            (*itcurr)->readyToRead();
                        }catch(DisconnectedException *e){
                            std::cout << "Disconnected in simpleeventloop" << std::endl;
                            writeset.erase(*itcurr);
                            //delete (*itcurr);
                            readset.erase(itcurr);
                            // modifed set, itcurr invalid, just wait for the select loop to go around again
                            break;
                        }
                    }
                }
            }
        }
    }
    
    void SimpleEventLoop::endEventLoop(){
        running = false;
    }

}

