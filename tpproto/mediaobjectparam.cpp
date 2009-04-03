/*  MediaObjectParameter - Media ObjectParameters.
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


#include "buffer.h"
#include "objectparametervisitor.h"

#include "mediaobjectparam.h"

namespace TPProto{
    
    /*! \brief Construct the MediaObjectParam with arguments.
    */
    MediaObjectParam::MediaObjectParam() : ObjectParameter(), media(){
    }
    
    /*! \brief Construct a MediaObjectParam from another MediaObjectParam.
    \param rhs The MediaObjectParam to copy.
    */
    MediaObjectParam::MediaObjectParam(const MediaObjectParam& rhs) :
            ObjectParameter(rhs), media(rhs.media){
    }
    
    /*! \brief Destruct the MediaObjectParam.
    */
    MediaObjectParam::~MediaObjectParam(){
    }
    
    /*! \brief Pack this MediaObjectParam into the Buffer.
    Packs an empty because the client can not modify media parameters.
    \param buf The Buffer to pack into.
    */
    void MediaObjectParam::packBuffer(Buffer* buf){
        buf->packString("");
    }
    
    /*! \brief Unpack this MediaObjectParam from the Buffer.
    \param buf The Buffer to unpack from.
    \return True if successful.
    */
    bool MediaObjectParam::unpackBuffer(Buffer* buf){
        media = buf->unpackString();
        return true;
    }
    
    /*! \brief Unpack the description of this MediaObjectParam from the Buffer.
    No data for the MediaObject in the Buffer.
    \param buf The Buffer to unpack the description from.
    \return True if successful.
    */
    bool MediaObjectParam::unpackDescBuffer(Buffer* buf){
        //nothing
        return true;
    }
    
    /*! \brief Clone this MediaObjectParam.
    \return The new MediaObjectParam.
    */
    ObjectParameter* MediaObjectParam::clone(){
        return new MediaObjectParam(*this);
    }
    
    /*! \brief Let the ObjectParameterVisitor visit this MediaObjectParam.
    \param opv The ObjectParameterVisitor that is visiting this parameter.
    */
    void MediaObjectParam::visit(ObjectParameterVisitor* opv){
        opv->visitObjectParameter(this);
    }
    
    /*! \brief Get the media location string.
    The media location string can either be an absolute URL, or relative to the Base URL in
    this game's GameFrame.
    \return The media location string.
    */
    std::string MediaObjectParam::getMediaString() const{
        return media;
    }
}
