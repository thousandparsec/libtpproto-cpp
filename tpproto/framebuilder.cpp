/*  FrameBuilder class
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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "framefactory.h"
#include "framecodec.h"
#include "protocollayer.h"
#include "buffer.h"
#include "getorderdesc.h"

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

#include "universe.h"
#include "galaxy.h"
#include "starsystem.h"
#include "planet.h"
#include "fleet.h"

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

    /*! \brief Builds a frame from it's type.
    Builds a frame subclass object that corresponds to the type given, or
    NULL if none is found.
    \param type The type of frame to build.
    \param data The Buffer of data that is the contents of the frame.
    \return The frame for the type, or NULL.
    */
    Frame* FrameBuilder::buildFrame(uint32_t type, Buffer* data){
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
            frame = buildObject(data->peekInt(4));
            break;

        case ft02_OrderDesc:
            frame = layer->getFrameFactory()->createOrderDescription();
            break;

        case ft02_Order:
            frame = buildOrder(data->peekInt(8));
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

        default:
            //others...
            break;
        }
        
        if(frame != NULL && !frame->unpackBuffer(data)){
            delete frame;
            frame = NULL;
        }
        return frame;
    }

    /*! \brief Creates the correct Object object from the type.

        Looks at the type in order to create the correct 
        Object based on the object type number.
        \param type The type of Object to build.
        \return The created Object.
    */
    Object* FrameBuilder::buildObject(uint32_t type){
        Object* ob;

        switch(type){
        case 0:
            ob = new Universe();
            break;
        case 1:
            ob = new Galaxy();
            break;
        case 2:
            ob = new StarSystem();
            break;
        case 3:
            ob = new Planet();
            break;
        case 4:
            ob = new Fleet();
            break;

        default:
            ob = NULL;
            break;
        }

        return ob;
    }

    /*! \brief Creates the correct Order type from the type.
    Fetches the order description from the server, then returns
    the correct type.
    \param type The Order type.
    */
    Order* FrameBuilder::buildOrder(uint32_t type){
        GetOrderDescription* god = layer->getFrameFactory()->createGetOrderDescription();
        god->addOrderType(type);
        uint32_t seq = layer->getFrameCodec()->sendFrame(god);
        OrderDescription* od = dynamic_cast<OrderDescription*>(layer->getFrameCodec()->recvFrames(seq).front());
        if(od != NULL){
            Order* f = layer->getFrameFactory()->createOrder();
            f->setOrderType(od);
            return f;
        }
        return NULL;
    }
}
