#ifndef TPPROTO_FRAMECODEC_H
#define TPPROTO_FRAMECODEC_H
/*  FrameCodec - changes network protocol to frame objects
 *
 *  Copyright (C) 2005  Lee Begg and the Thousand Parsec Project
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

#include <pthread.h>
#include <string>
#include <set>
#include <map>
#include <list>
/*! \file
  \brief Declares the FrameCodec class.

  It is the main file you will need to include to access low level functions.
*/

namespace TPProto{

  class TPSocket;
  class Frame;
  class AsyncFrameListener;
  class Logger;
    class ProtocolLayer;
    class GetObjects;
    class Object;
    class OrderDescription;
    class GetOrder;
    class Order;
    class RemoveOrder;
    class GetBoard;
    class Board;
    class GetMessage;
    class Message;
    class RemoveMessage;

    /*! \brief FrameCodec is the main working class the lower layer of libtpproto-cpp.

        The FrameCodec class is responsible for handling the running of
        the protocol.  It interacts with the Thousand Parsec servers and 
        avoids a lot of unneccessary handling by the client and higher 
        layers.  The FrameCodec class is not the best way for a client
        to access a tp server, use the upper layers for that, but it can
        be used if necessary. A ProtocolLayer must be set (by setting
        this object as the FrameCodec for the ProtocolLayer).
    */
  class FrameCodec{
  public:
    FrameCodec();
    ~FrameCodec();

    //config & setup

    void setSocket(TPSocket * nsock);
    void setAsyncFrameListener(AsyncFrameListener* afl);
    void setLogger(Logger* nlog);
        void setProtocolLayer(ProtocolLayer* pl);

    //status
    int getStatus();

    //polling
    void pollForAsyncFrames();
    
    //send and receive frames
    uint32_t sendFrame(Frame * f);
    std::list<Frame*> recvFrames(uint32_t seqnum);

  private:
    Frame* recvOneFrame();
    void clearIncomingFrames();

    TPSocket * sock;
    AsyncFrameListener* asynclistener;
    Logger* logger;
        ProtocolLayer* layer;
    int status;
    int version;
    int nextseqnum;

    pthread_mutex_t* rmutex;
    pthread_mutex_t* wmutex;
    pthread_mutex_t* smutex;

    std::map<uint32_t, std::pair<uint32_t, std::list<Frame*>* > > incomingframes;

  };

}

#endif
