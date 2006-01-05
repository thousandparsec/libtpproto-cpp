/*  CacheNoneMethod - Methods of caching Frames that doesn't class
 *
 *  Copyright (C) 2006  Lee Begg and the Thousand Parsec Project
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

#include <list>
#include <set>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cache.h"
#include "protocollayer.h"
#include "getbyid.h"
#include "framecodec.h"
#include "getidsequence.h"
#include "idsequence.h"

#include "cachenonemethod.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    CacheNoneMethod::CacheNoneMethod() : CacheMethod(){
    }

    /*! \brief Copy Constructor.
        No fields to copy or zero.
    */
    CacheNoneMethod::CacheNoneMethod(const CacheNoneMethod& rhs) : CacheMethod(){
    }

    /*! \brief Destructor, virtual
    */
    CacheNoneMethod::~CacheNoneMethod(){
    }

    void CacheNoneMethod::update(){
    }

    Frame* CacheNoneMethod::getById(uint32_t id){
        GetById* gbi = cache->createGetByIdFrame();
        gbi->addId(id);
        uint32_t seqnum = protocol->getFrameCodec()->sendFrame(gbi);
        delete gbi;
        std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
        Frame * reply = NULL;
        if(replies.size() >= 1){
            reply = replies.front();
        }

        if(reply == NULL || reply->getType() == ft02_Fail){
            //logger->error("The returned frame is failure or null");
        }

        return reply;

    }

    void CacheNoneMethod::markInvalid(uint32_t id){
    }

    std::set<uint32_t> CacheNoneMethod::getAllIds(){
        std::set<uint32_t> out;
        GetIdSequence *frame = cache->createGetIdSequenceFrame();
        if(frame != NULL){
            frame->setCount(8737); // When this code is shifted out, this should be in a loop to get all the items
            uint32_t seqnum = protocol->getFrameCodec()->sendFrame(frame);
            
            std::list<Frame*> replies = protocol->getFrameCodec()->recvFrames(seqnum);
            Frame * reply = NULL;
            if(replies.size() >= 1){
                reply = replies.front();
            }
            
            if(reply != NULL && reply->getType() != ft02_Fail){
                std::map<uint32_t, uint64_t> ids = static_cast<IdSequence*>(reply)->getIds();
                for(std::map<uint32_t, uint64_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
                    out.insert(itcurr->first);
                }
            }else{
                //logger->debug("Expecting idsequence frame, but got %d instead", reply->getType());
            }
        }
        return out;
    }

    CacheMethod* CacheNoneMethod::clone(){
        return new CacheNoneMethod(*this);
    }

}
