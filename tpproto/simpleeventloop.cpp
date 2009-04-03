/*  SimpleEventLoop - an example simple event loop.
 *
 *  Copyright (C) 2008, 2009  Lee Begg and the Thousand Parsec Project
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

#include "tpsocket.h"

#include "simpleeventloop.h"

namespace TPProto{
    
    /*! \brief Implements the '>' operator for ordering the Timers.
    \param rhs The Timer to compare against.
    \return True if the expiration time of this Timer is greater than the argument Timer.
    */
    bool Timer::operator>(const Timer& rhs) const{
        return expiretime > rhs.expiretime;
    }

    /*! \brief Constructs the SimpleEventLoop.
    */
    SimpleEventLoop::SimpleEventLoop() : running(false), readset(), writeset(), timers(){
    }
    
    /*! \brief Desctructs the SimpleEventLoop.
    Frees the TPSockets that are still open.
    */
    SimpleEventLoop::~SimpleEventLoop(){
        for(std::set<TPSocket*>::iterator itcurr = readset.begin(); itcurr != readset.end();
                ++itcurr){
            delete (*itcurr);
        }
        readset.clear();
        writeset.clear();
    }

    /*! \brief Adds a TPSocket to the reading list.
    \param sock The TPSocket that wants to read.
    The TPSocket remains in the list until closed or removed.
    */
    void SimpleEventLoop::listenForSocketRead(TPSocket* sock){
        readset.insert(sock);
    }
    
    /*! \brief Adds a TPSocket to the writing list.
    \param sock The TPSocket that wants to write.
    After it has written, it is removed from the writing list. To write more,
    the TPSocket has to be re-added to the writing list.
    */
    void SimpleEventLoop::listenForSocketWrite(TPSocket* sock){
        writeset.insert(sock);
    }
    
    /*! \brief Set a timer to be called.
    \param interval Time in seconds until the callback is called.
    \param callback The callback to call when the timer expires.
    \return The TimerConnection that can be used to manage the timer
    */
    TimerConnection SimpleEventLoop::setTimer(uint32_t interval, const TimerSignal::slot_type& callback){
        Timer timer;
        timer.expiretime = time(NULL) + interval;
        timer.signal = new TimerSignal();
        TimerConnection tc = timer.signal->connect(callback);
        timers.push(timer);
        return tc;
    }
            
    /*! \brief Runs the EventLoop.
    Call to start processing event. This does not exit until after SimpleEventLoop::endEventLoop() is called.
    */
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
    
    /*! \brief End the eventloop processing.
    Cause the call to SimpleEventLoop::runEventLoop() to exit the next time it loops.
    */
    void SimpleEventLoop::endEventLoop(){
        running = false;
    }

}

