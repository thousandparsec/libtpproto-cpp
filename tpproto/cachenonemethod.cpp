/*  CacheNoneMethod - Methods of caching Frames that doesn't class
 *
 *  Copyright (C) 2006, 2008  Lee Begg and the Thousand Parsec Project
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
#include <boost/bind.hpp>

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

    void CacheNoneMethod::getById(uint32_t id){
        GetById* gbi = cache->createGetByIdFrame();
        gbi->addId(id);
        protocol->getFrameCodec()->sendFrame(boost::shared_ptr<GetById>(gbi), boost::bind(&CacheNoneMethod::receiveItem, this, _1));
    }

    void CacheNoneMethod::markInvalid(uint32_t id){
    }

    void CacheNoneMethod::getIdList(){
        
        GetIdSequence *frame = cache->createGetIdSequenceFrame();
        if(frame != NULL){
            frame->setCount(8737); // When this code is shifted out, this should be in a loop to get all the items
            protocol->getFrameCodec()->sendFrame(boost::shared_ptr<Frame>(frame), boost::bind(&CacheNoneMethod::receiveIdList, this, _1));
            
           delete frame; 
        }
        
    }

    CacheMethod* CacheNoneMethod::clone(){
        return new CacheNoneMethod(*this);
    }

    void CacheNoneMethod::receiveItem(Frame* frame){
        if(frame->getType() != ft02_Fail){
            cache->newItem(boost::shared_ptr<Frame>(frame));
        }else{
            delete frame;
        }
    }
    
    void CacheNoneMethod::receiveIdList(Frame* frame){
        std::set<uint32_t> out;
        if(frame != NULL && frame->getType() != ft02_Fail){
            std::map<uint32_t, uint64_t> ids = static_cast<IdSequence*>(frame)->getIds();
            for(std::map<uint32_t, uint64_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
                out.insert(itcurr->first);
            }
        }
        newIdList(out);
    }
    
}
