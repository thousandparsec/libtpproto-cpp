/*  FrameCodec class
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tpsocket.h"
#include "buffer.h"
#include "framefactory.h"
#include "framebuilder.h"
#include "protocollayer.h"

#include "frame.h"
#include "sequence.h"
#include "timeremaining.h"
#include "asyncframelistener.h"

#include "logger.h"
#include "silentlogger.h"


#include "framecodec.h"

namespace TPProto {

  /*! \brief Constructs object and sets up defaults.

  Defaults are
    - no TPSocket
    - no AsyncFrameListener
    - SilentLogger for the Logger
    - version unknown
  */
  FrameCodec::FrameCodec(){
    sock = NULL;
    asynclistener = NULL;
    logger = new SilentLogger();
    status = 0;
        version = 0;
    nextseqnum = 1; // should be random

    rmutex = new pthread_mutex_t();
    pthread_mutex_init(rmutex, NULL);
    wmutex = new pthread_mutex_t();
    pthread_mutex_init(wmutex, NULL);
    smutex = new pthread_mutex_t();
    pthread_mutex_init(smutex, NULL);
  }

  /*! \brief Destructor.
   */
  FrameCodec::~FrameCodec(){
    if(sock != NULL){
      delete sock;
    }
    delete logger;

        clearIncomingFrames();

    pthread_mutex_destroy(rmutex);
    delete rmutex;
    pthread_mutex_destroy(wmutex);
    delete wmutex;
    pthread_mutex_destroy(smutex);
    delete smutex;
  }

  /*! \brief Sets the TPSocket to be used for communicating with the server.

  \param nsock The TPSocket to be used.
  */
  void FrameCodec::setSocket(TPSocket * nsock){
    if(sock != NULL)
      delete sock;
    sock = nsock;
        version = 0;
  }

  /*! \brief Sets the AsyncFrameListener.

  Only one AsyncFrameListener is allowed currently.  When a new listener
  is set, the old one if present is deleted.
  \param afl The AsyncFrameListener to use.
  */
  void FrameCodec::setAsyncFrameListener(AsyncFrameListener* afl){
    asynclistener = afl;
  }

  /*! \brief Sets the Logger.

  This method sets a new Logger.  The old Logger is deleted.  If the pointer
  to the new Logger is NULL, the default SilentLogger is used.
  \param nlog The new Logger to use, or NULL
  */  
  void FrameCodec::setLogger(Logger* nlog){
    delete logger;
    logger = nlog;
    if(logger == NULL){
      logger = new SilentLogger();
    }
    logger->debug("Logger set");
  }

    /*! \brief Sets the ProtocolLayer.

    This method sets the ProtocolLayer.
    \param pl The ProtocolLayer to use.
    */
    void FrameCodec::setProtocolLayer(ProtocolLayer* pl){
        layer = pl;
        logger->debug("Protocol Layer set");
    }

  /*! \brief Gets the status of the connection.
    \return The status (int).
  */
  int FrameCodec::getStatus(){
    if(sock == NULL || !sock->isConnected()){
      status = 0;
    }
    return status;
  }


  /*! \brief Checks the connection for any AsyncFrames.

  This should be called occasionally when there hasn't been any
  interaction between the client and the server.  Most of the time,
  asynchronous frames will be picked up while processing other frames,
  but in times of inactivity it a good idea to poll this.  The acturacy
  of data such as any asynchronous TimeRemaining frame will on how close to
  the time the frame was received to when this method (or any method that
  receives any frame from the server) is called.
  */
  void FrameCodec::pollForAsyncFrames(){
    if(status == 3){
    if(pthread_mutex_trylock(rmutex) == 0){
      if(sock->poll()){
        pthread_mutex_unlock(rmutex);
	Frame* frame = recvOneFrame();
	if(frame != NULL){
	  if(frame->getSequenceNumber() == 0){
	    // async frame, send it on and try again.
	    if(asynclistener != NULL){
	      if(frame->getType() == ft02_Time_Remaining){
		asynclistener->recvTimeRemaining((TimeRemaining*)frame);
	      }else{
		logger->warning("Unknown Async Frame type, ignoring");
	      }
	    }else{
	      logger->warning("No AsyncFrameListener, discarding frame");
	    }
	  }else{
	    logger->error("Was expecting Async frame but received a frame of type %d with sequence number %d",
			  frame->getType(), frame->getSequenceNumber());
            pthread_mutex_lock(smutex);
            if(incomingframes.find(frame->getSequenceNumber()) != incomingframes.end() &&
                    incomingframes[frame->getSequenceNumber()].first > incomingframes[frame->getSequenceNumber()].second->size()){
                std::list<Frame*>* framelist = incomingframes[frame->getSequenceNumber()].second;
                if(frame->getType() == ft02_Sequence){
                    incomingframes[frame->getSequenceNumber()].first = ((Sequence*)frame)->getNumber();
                    delete frame;
                }else{
                    framelist->push_back(frame);
                }
            }
            pthread_mutex_unlock(smutex);
	  }

	  delete frame;
	}else{
	  logger->debug("Frame was null, should have been an async frame");
	}
      }else{
        pthread_mutex_unlock(rmutex);
        //nothing on socket to get.
    }

    }

    }else{
      logger->warning("Trying to poll for async frames but not connected or logged in");
    }
  }

  /*! \brief Sends a Frame.

  Packs the Frame into a Buffer and sends it via the TPSocket.  Sets the 
  sequence number and increments the sequence number counter.
  \param f The Frame to send.
  */
  uint32_t FrameCodec::sendFrame(Frame *f){
    if(sock->isConnected()){
      Buffer *data = new Buffer();
      f->packBuffer(data);
      Buffer *header = new Buffer();
            uint32_t real_seqnum = nextseqnum;
            header->createHeader(f->getProtocolVersion(), real_seqnum, f->getType(), data->getLength());
        pthread_mutex_lock(wmutex);
      sock->send(header->getData(), header->getLength(), data->getData(), data->getLength());
        nextseqnum++;
        if(nextseqnum == 0)
            nextseqnum++;
        pthread_mutex_unlock(wmutex);
      delete data;
      delete header;

        pthread_mutex_lock(smutex);
        incomingframes[real_seqnum] = std::pair<uint32_t, std::list<Frame*>* >(1, new std::list<Frame*>());
        pthread_mutex_unlock(smutex);
      
            return real_seqnum;
    }
        return 0;
  }

  /*! \brief Receives a normal Frame.

  Receives a normal Frame from the TPSocket, passing any asynchronous frames
  to the AsyncFrameListener until a normal frame is received. Only call this once 
  for each sequence number
    \param seqnum The sequence number for the frame to get.
  \return List of received Frames with the sequence number.
  */
    std::list<Frame*> FrameCodec::recvFrames(uint32_t seqnum){
        pthread_mutex_lock(smutex);
        if(seqnum == 0 || incomingframes[seqnum].first == 0){
            pthread_mutex_unlock(smutex);
            return std::list<Frame*>();
        }
        while(incomingframes[seqnum].second != NULL && incomingframes[seqnum].first > incomingframes[seqnum].second->size()){
            pthread_mutex_unlock(smutex);
    Frame* frame = recvOneFrame();
    if(frame != NULL && frame->getSequenceNumber() == 0){
      // async frame, send it on and try again.
      
      if(asynclistener != NULL){
	if(frame->getType() == ft02_Time_Remaining){
	  asynclistener->recvTimeRemaining((TimeRemaining*)frame);
	}
      }
      
      if(frame != NULL){
	delete frame;
      }
      
            }else if(frame == NULL){
                // connection closed, return what we have
                pthread_mutex_lock(smutex);
                break;
            }else{
                pthread_mutex_lock(smutex);
                std::list<Frame*>* framelist = incomingframes[frame->getSequenceNumber()].second;
                if(frame->getType() == ft02_Sequence){
                    incomingframes[frame->getSequenceNumber()].first = ((Sequence*)frame)->getNumber();
                    delete frame;
                }else{
                    framelist->push_back(frame);
                }
                pthread_mutex_unlock(smutex);
            }
            pthread_mutex_lock(smutex);
        }

        if(incomingframes[seqnum].second == NULL){
            pthread_mutex_unlock(smutex);
            return std::list<Frame*>();
        }else{
            std::list<Frame*> rtv = *(incomingframes[seqnum].second);
            delete incomingframes[seqnum].second;
            incomingframes.erase(seqnum);
            pthread_mutex_unlock(smutex);
            return rtv;
        }

    }

  /*! \brief Receives one Frame from the network.

  Grabs one Frame from the TPSocket.
  \return The received Frame or NULL.
  */
  Frame* FrameCodec::recvOneFrame(){
    if(sock->isConnected()){
      char* head, *body;
        pthread_mutex_lock(rmutex);
      int rlen = sock->recvHeader(16, head);
      if(rlen != 16){
	//now what?
	logger->warning("Could not read whole header");
	delete head;
	return NULL;
      }
      
      int len, type, sequ, fver;
      
      Buffer * header = new Buffer();
      header->setData(head, rlen);
 
      if(!header->readHeader(fver, sequ, type, len)){
	// invalid header
	logger->warning("Header invalid");
	delete header;
	return NULL;
      }
      delete header;
      
        if(fver != 2 && fver != 3){
	logger->warning("Wrong verison of protocol, ver: %d sequ: %d type: %d len: %d", fver, sequ, type, len);
	sock->disconnect();
	status = 0;
	return NULL;
      }
      
        if(fver != version){
            //version mismatch
            if(version == 0 && fver > 1 && fver <= 3){
                version = fver;
                layer->getFrameFactory()->setProtocolVersion(version);
            }else{
                logger->warning("Wrong verison of protocol (%d), server got it wrong, ", fver);
                sock->disconnect();
                status = 0;
                return NULL;
            }
        }

      rlen = sock->recvBody(len, body);
      if(rlen != len){
	//again, now what?
	logger->warning("Could not read whole body");
	delete body;
	return NULL;
      }
      pthread_mutex_unlock(rmutex);
      Buffer *data = new Buffer();
      data->setData(body, rlen);
      
      Frame* frame = layer->getFrameBuilder()->buildFrame(type, data);

      if(frame == NULL){
	//others...
	logger->warning("Received frame of type %d but don't know what to do, setting return value to NULL", type);
      }else{
	frame->setProtocolVersion(fver);
	frame->setSequenceNumber(sequ);
      }
      
      delete data;

      return frame;
    }
    return NULL;
  }

    void FrameCodec::clearIncomingFrames(){
        pthread_mutex_lock(smutex);
        for(std::map<uint32_t, std::pair<uint32_t, std::list<Frame*>* > >::iterator outeriter = incomingframes.begin();
                outeriter != incomingframes.end(); ++outeriter){
            std::list<Frame*> tmplist = *(outeriter->second.second);
            for(std::list<Frame*>::iterator inneriter = tmplist.begin();
                    inneriter != tmplist.end(); ++inneriter){
                delete *inneriter;
            }
            delete outeriter->second.second;
        }
        incomingframes.clear();
        pthread_mutex_unlock(smutex);
    }
    
}// namespace

