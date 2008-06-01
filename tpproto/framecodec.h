#ifndef TPPROTO_FRAMECODEC_H
#define TPPROTO_FRAMECODEC_H
/*  FrameCodec - changes network protocol to frame objects
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

#include <string>
#include <set>
#include <map>
#include <list>
#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>

#include <tpproto/connection.h>

/*! \file
  \brief Declares the FrameCodec class.

  It is the main file you will need to include to access low level functions.
*/

namespace TPProto{

  class Frame;
  class AsyncFrameListener;
  class Logger;
    class ProtocolLayer;
    
    typedef boost::signal<void (Frame*)> FrameSignal;
    typedef boost::signals::connection FrameConnection;

    /*! \brief FrameCodec is the main working class the lower layer of libtpproto-cpp.

        The FrameCodec class is responsible for handling the running of
        the protocol.  It interacts with the Thousand Parsec servers and 
        avoids a lot of unneccessary handling by the client and higher 
        layers.  The FrameCodec class is not the best way for a client
        to access a tp server, use the upper layers for that, but it can
        be used if necessary. A ProtocolLayer must be set (by setting
        this object as the FrameCodec for the ProtocolLayer).
    */
  class FrameCodec : public Connection{
  public:
    FrameCodec();
    ~FrameCodec();

    //config & setup

    void setAsyncFrameListener(AsyncFrameListener* afl);
    void setLogger(Logger* nlog);
        void setProtocolLayer(ProtocolLayer* pl);

    //status
    int getStatus();
    
    //send and receive frames
    //uint32_t sendFrame(Frame * f);
    //std::list<Frame*> recvFrames(uint32_t seqnum);
    FrameConnection sendFrame(boost::shared_ptr<Frame>  f, const FrameSignal::slot_type& callback);
    
    //Connection related methods
    void readyToRead();
    void readyToSend();
    
    //receive built frame
    void receivedFrame(Frame* frame);

  private:
    void recvOneFrame();

    AsyncFrameListener* asynclistener;
    Logger* logger;
        ProtocolLayer* layer;
    int status;
    int version;
    int nextseqnum;

    char* rheaderbuff;
    char* rdatabuff;
    uint32_t rbuffused;

    std::map<uint32_t, FrameSignal* > framesignals;
    std::map<uint32_t, uint32_t> expectedframes;

  };

}

#endif
