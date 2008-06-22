/*  FrameBuilder class
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

#include <boost/bind.hpp>
#include <iostream>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "framefactory.h"
#include "framecodec.h"
#include "protocollayer.h"
#include "buffer.h"
#include "orderdesccache.h"
#include "objectdesccache.h"
#include "commanddesccache.h"

#include "okframe.h"
#include "failframe.h"
#include "sequence.h"
#include "redirect.h"
#include "featuresframe.h"
#include "object.h"
#include "objectidslist.h"
#include "orderdesc.h"
#include "ordertypeslist.h"
#include "order.h"
#include "timeremaining.h"
#include "board.h"
#include "boardidslist.h"
#include "message.h"
#include "resourcedesc.h"
#include "resourcetypeslist.h"
#include "player.h"
#include "category.h"
#include "categoryidslist.h"
#include "design.h"
#include "designidslist.h"
#include "component.h"
#include "componentidslist.h"
#include "property.h"
#include "propertyidslist.h"
#include "gameinfo.h"
#include "objecttypeslist.h"
#include "objectdesc.h"
#include "logmessage.h"
#include "commanddesc.h"
#include "commandtypeslist.h"
#include "command.h"
#include "commandresult.h"

#include "framebuilder.h"

namespace TPProto{

    /*! \brief Constructs object and sets up defaults.
    */
    FrameBuilder::FrameBuilder(){
    }

    /*! \brief Destructor.
    */
    FrameBuilder::~FrameBuilder(){
    }

    /*! \brief Sets the ProtocolLayer.
    \param pl The ProtocolLayer to use.
    */
    void FrameBuilder::setProtocolLayer(ProtocolLayer* pl){
        layer = pl;
    }

    void FrameBuilder::setOrderDescCache(OrderDescCache* odc){
        orderdesccache = odc;
    }
    
    void FrameBuilder::setObjectDescCache(ObjectDescCache* odc){
        objectdesccache = odc;
    }

    void FrameBuilder::setCommandDescCache(CommandDescCache* cdc){
        commanddesccache = cdc;
    }
    
    /*! \brief Builds a frame from it's type.
    Builds a frame subclass object that corresponds to the type given, or
    NULL if none is found.
    Frame sent to FrameCodec::receivedFrame().
    \param type The type of frame to build.
    \param data The Buffer of data that is the contents of the frame.
    \param ftver The version of the frametype
    \param seqnum The sequence number
    */
    void FrameBuilder::buildFrame(uint32_t type, Buffer* data, uint32_t ftver, uint32_t seqnum){
        Frame* frame = NULL;
        // may need to switch on version too
        switch(type){
        case ft02_OK:
            frame = layer->getFrameFactory()->createOk();
            break;

        case ft02_Fail:
            frame = layer->getFrameFactory()->createFail();
            break;

        case ft02_Sequence:
            frame = layer->getFrameFactory()->createSequence();
            break;

        case ft02_Object:
            frame = layer->getFrameFactory()->createObject();
            break;

        case ft02_OrderDesc:
            frame = layer->getFrameFactory()->createOrderDescription();
            break;

        case ft02_Order:
            frame = layer->getFrameFactory()->createOrder();
            break;

        case ft02_Time_Remaining:
            frame = layer->getFrameFactory()->createTimeRemaining();
            break;

        case ft02_Board:
            frame = layer->getFrameFactory()->createBoard();
            break;

        case ft02_Message:
            frame = layer->getFrameFactory()->createMessage();
            break;

        case ft02_ResDesc:
            frame = layer->getFrameFactory()->createResourceDescription();
            break;
            
        case ft03_Redirect:
            frame = layer->getFrameFactory()->createRedirect();
            break;
            
        case ft03_Features:
            frame = layer->getFrameFactory()->createFeatures();
            break;
            
        case ft03_ObjectIds:
            frame = layer->getFrameFactory()->createObjectIdsList();
            break;
            
        case ft03_OrderTypes:
            frame = layer->getFrameFactory()->createOrderTypesList();
            break;
            
        case ft03_BoardIds:
            frame = layer->getFrameFactory()->createBoardIdsList();
            break;
            
        case ft03_ResourceTypes:
            frame = layer->getFrameFactory()->createResourceTypesList();
            break;
            
        case ft03_Player:
            frame = layer->getFrameFactory()->createPlayer();
            break;
            
        case ft03_Category:
            frame = layer->getFrameFactory()->createCategory();
            break;
            
        case ft03_CategoryIds:
            frame = layer->getFrameFactory()->createCategoryIdsList();
            break;
            
        case ft03_Design:
            frame = layer->getFrameFactory()->createDesign();
            break;
            
        case ft03_DesignIds:
            frame = layer->getFrameFactory()->createDesignIdsList();
            break;
            
        case ft03_Component:
            frame = layer->getFrameFactory()->createComponent();
            break;
            
        case ft03_ComponentIds:
            frame = layer->getFrameFactory()->createComponentIdsList();
            break;
            
        case ft03_Property:
            frame = layer->getFrameFactory()->createProperty();
            break;
            
        case ft03_PropertyIds:
            frame = layer->getFrameFactory()->createPropertyIdsList();
            break;
            
        case ft04_GameInfo:
            frame = layer->getFrameFactory()->createGameInfo();
            break;
            
        case ft04_ObjectDesc:
            frame = layer->getFrameFactory()->createObjectDescription();
            break;
            
        case ft04_ObjectTypes_List:
            frame = layer->getFrameFactory()->createObjectTypesList();
            break;

        case ftad_LogMessage:
            frame = layer->getFrameFactory()->createLogMessage();
            break;

        case ftad_CommandDesc:
            frame = layer->getFrameFactory()->createCommandDescription();
            break;

        case ftad_CommandTypes_List:
            frame = layer->getFrameFactory()->createCommandTypesList();
            break;

        case ftad_CommandResult:
            frame = layer->getFrameFactory()->createCommandResult();
            break;

        default:
            //others...
            break;
        }
        
        if(frame != NULL){
            frame->setSequenceNumber(seqnum);
            //frame->setFrameTypeVersion(ftver); //TODO when frame supports it
            if(type == ft02_Object){
                objectdesccache->requestObjectDescription(data->peekInt(4), boost::bind(&FrameBuilder::processObjectDescription, this, frame, data, _1));
            }else if(type == ft02_Order){
                orderdesccache->requestOrderDescription(data->peekInt(8), boost::bind(&FrameBuilder::processOrderDescription, this, frame, data, _1));
            }else if(type == ftad_Command){
                commanddesccache->requestCommandDescription(data->peekInt(4), boost::bind(&FrameBuilder::processCommandDescription, this, frame, data, _1));
            }else{
                if(frame->unpackBuffer(data)){
                    delete data;
                    layer->getFrameCodec()->receivedFrame(frame);
                }else{
                    delete frame;
                    delete data;
                    //TODO create a fail frame with local error, and give to framecodec
                    std::cerr << "Failed to unpack Buffer" << std::endl;
                }
            }
            
        }
    }

    void FrameBuilder::processOrderDescription(Frame* frame, Buffer* data, boost::shared_ptr<OrderDescription> od){
        if(od){
            Order* order = static_cast<Order*>(frame);
            order->setOrderType(od);
            if(order->unpackBuffer(data)){
                delete data;
                layer->getFrameCodec()->receivedFrame(frame);
            }else{
                delete frame;
                delete data;
                //fail frame as above
            }
        }else{
            delete frame;
            delete data;
            //fail frame as above
        }
    }
    
    void FrameBuilder::processObjectDescription(Frame* frame, Buffer* data, boost::shared_ptr<ObjectDescription> od){
        if(od){
            Object* object = static_cast<Object*>(frame);
            object->setObjectType(od);
            if(object->unpackBuffer(data)){
                delete data;
                layer->getFrameCodec()->receivedFrame(frame);
            }else{
                delete frame;
                delete data;
                //fail frame as above
            }
        }else{
            delete frame;
            delete data;
            //fail frame as above
        }
    }

    void FrameBuilder::processCommandDescription(Frame* frame, Buffer* data, boost::shared_ptr<CommandDescription> cd){
        if(cd){
            Command* command = static_cast<Command*>(frame);
            command->setCommandType(cd);
            if(command->unpackBuffer(data)){
                delete data;
                layer->getFrameCodec()->receivedFrame(frame);
            }else{
                delete frame;
                delete data;
                //fail frame as above
            }
        }else{
            delete frame;
            delete data;
            //fail frame as above
        }
    }

}
