/*  MediaObjectParameter - Media ObjectParameters.
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


#include "buffer.h"
#include "objectparametervisitor.h"

#include "mediaobjectparam.h"

namespace TPProto{
    
    MediaObjectParam::MediaObjectParam() : ObjectParameter(), media(){
    }
    
    MediaObjectParam::MediaObjectParam(const MediaObjectParam& rhs) :
            ObjectParameter(rhs), media(rhs.media){
    }
    
    MediaObjectParam::~MediaObjectParam(){
    }
    
    void MediaObjectParam::packBuffer(Buffer* buf){
        buf->packString("");
    }
    
    bool MediaObjectParam::unpackBuffer(Buffer* buf){
        media = buf->unpackString();
        return true;
    }
    
    bool MediaObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    ObjectParameter* MediaObjectParam::clone(){
        return new MediaObjectParam(*this);
    }
    
    void MediaObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    std::string MediaObjectParam::getMediaString() const{
        return media;
    }
}
