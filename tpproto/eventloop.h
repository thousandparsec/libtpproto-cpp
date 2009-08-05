#ifndef TPPROTO_EVENTLOOP_H
#define TPPROTO_EVENTLOOP_H
/*  EventLoop - Interface to the client program's event loop.
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

#include <boost/signal.hpp>

/*! \file
  \brief Declares the EventLoop abstraction.
 */

namespace TPProto{

    class TPSocket;
    
    /*! Type for TimerSignal
    */
    typedef boost::signal<void (void)> TimerSignal;
    typedef boost::signals::connection TimerConnection;
    
    /*! \brief Interface to the client's event loop.
    
    This class should be subclassed in every client to provide a 
    way of integrating with the existing event loop, or a new 
    event loop if required.
    */
    class EventLoop{
        public:
            /*! Required virtual destructor.
            */
            virtual ~EventLoop(){};

            /*! \brief Gives a TPSocket to the event loop to be alerted when it's ready to read.
            
            This needs to be implemented to have the TPSocket::isReadyToRead() method
            called when there is data waiting for the socket.
            
            The socket is removed when it is disconnected.
            
            The TPSocket::isReadyToRead() method can throw a DisconnectedException, which should be
            caught before returning to the underlying event loop.
            
            \param sock The TPSocket to check
            */
            virtual void listenForSocketRead(TPSocket* sock) = 0;
            
            /*! \brief Gives a TPSocket to the event loop to be alerted when it's ready to write.
            
            This needs to be implemented to have the TPSocket::isReadyToSend() method
            called when the socket is ready to send data.
            
            This is a single shot, and should be disabled from from being triggered again
            <i>before</i> calling TPSocket::isReadyToSend().
            
            The TPSocket::isReadyToSend() method can throw a DisconnectedException, which should be
            caught before returning to the underlying event loop.
            
            \param sock The TPSocket to check
            */
            virtual void listenForSocketWrite(TPSocket* sock) = 0;
            
            /*! \brief Sets up a timer to call in a given interval
            \param interval The time interval before calling, in seconds
            \param callback The slot/callback to call when the timer expires.
            \return A TimerConnection to be managed by the caller.
            */
            virtual TimerConnection setTimer(uint32_t interval, const TimerSignal::slot_type& callback) = 0;
    };

}

#endif
