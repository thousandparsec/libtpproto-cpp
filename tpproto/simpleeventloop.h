#ifndef TPPROTO_SIMPLEEVENTLOOP_H
#define TPPROTO_SIMPLEEVENTLOOP_H
/*  SimpleEventLoop - A simple sample event loop.
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

#include <set>
#include <queue>
#include <functional>
#include <algorithm>

#include <tpproto/eventloop.h>

/*! \file
  \brief Declares the SimpleEventLoop class.
 */

namespace TPProto{

    /*! \brief Encapulated class for timers
    */
    class Timer{
        public:
            TimerSignal *signal;
            int32_t expiretime;
            bool operator>(const Timer & rhs) const;
    };
    
    /*! \brief Simple event loop example.
    
    This class is a subclass of EventLoop to demostrate how to interact
    with an existing event loop. The whole eventloop is implemented in this class.
    
    This SimpleEventLoop is used by the tests and some example programs.
    */
    class SimpleEventLoop{
        public:
            SimpleEventLoop();
            virtual ~SimpleEventLoop();

            virtual void listenForSocketRead(TPSocket* sock);
            virtual void listenForSocketWrite(TPSocket* sock);
            virtual TimerConnection setTimer(uint32_t interval, const TimerSignal::slot_type& callback);
            
            void runEventLoop();
            void endEventLoop();
            
        private:
            volatile bool running;
            
            std::set<TPSocket*> readset;
            std::set<TPSocket*> writeset;
            
            std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer> > timers;
            
    };

}

#endif
