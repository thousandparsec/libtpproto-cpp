/*  FrameFactory class
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

#ifndef VERSION
#define VERSION "0.0.0"
#endif

#include "protocollayer.h"

// Frame Types
#include "frame.h"

#include "okframe.h"
#include "failframe.h"
#include "sequence.h"
#include "connect.h"
#include "login.h"
#include "redirect.h"
#include "getfeatures.h"
#include "featuresframe.h"
#include "ping.h"
#include "getobjectbyid.h"
#include "getobjectbypos.h"
#include "object.h"
#include "getobjectidslist.h"
#include "getobjectidsbypos.h"
#include "getobjectidsbycontainer.h"
#include "objectidslist.h"
#include "getorderdesc.h"
#include "orderdesc.h"
#include "getordertypeslist.h"
#include "ordertypeslist.h"
#include "getorder.h"
#include "order.h"
#include "removeorder.h"
#include "probeorder.h"
#include "gettime.h"
#include "timeremaining.h"
#include "getboard.h"
#include "board.h"
#include "getboardidslist.h"
#include "boardidslist.h"
#include "getmessage.h"
#include "message.h"
#include "removemessage.h"
#include "getresourcedesc.h"
#include "resourcedesc.h"
#include "getresourcetypeslist.h"
#include "resourcetypeslist.h"
#include "getplayer.h"
#include "player.h"
#include "getcategory.h"
#include "category.h"
#include "addcategory.h"
#include "removecategory.h"
#include "getcategoryidslist.h"
#include "categoryidslist.h"
#include "getdesign.h"
#include "design.h"
#include "adddesign.h"
#include "modifydesign.h"
#include "removedesign.h"
#include "getdesignidslist.h"
#include "designidslist.h"
#include "getcomponent.h"
#include "component.h"
#include "getcomponentidslist.h"
#include "componentidslist.h"
#include "getproperty.h"
#include "property.h"
#include "getpropertyidslist.h"
#include "propertyidslist.h"

#include "framefactory.h"

namespace TPProto {

    /*! \brief Constructs object and sets up defaults.

    Defaults are
        - version 3 of the protocol
    */
    FrameFactory::FrameFactory(){
        pver = 3; // TP03
    }

    /*! \brief Require virtual destructor.
    */
    FrameFactory::~FrameFactory(){
    }

    /*! \brief Sets the protocol version the frames should have.
    \param pv The protocol version number.
    */
    void FrameFactory::setProtocolVersion(int pv){
        pver = pv;
    }
    
    /*! \brief Gets the protocol version.
    \return The protocol version number.
    */
    int FrameFactory::getProtocolVersion() const{
        return pver;
    }

    /*! \brief Sets the ProtocolLayer.

    This method sets the ProtocolLayer.
    \param pl The ProtocolLayer to use.
    */
    void FrameFactory::setProtocolLayer(ProtocolLayer* pl){
        layer = pl;
    }

    OkFrame* FrameFactory::createOk(){
        OkFrame* ok = new OkFrame();
        ok->setProtocolVersion(pver);
        return ok;
    }

    FailFrame* FrameFactory::createFail(){
        FailFrame* f = new FailFrame();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Sequence* FrameFactory::createSequence(){
        Sequence* f = new Sequence();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Connect* FrameFactory::createConnect(){
        Connect* f = new Connect();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Login* FrameFactory::createLogin(){
        Login* f = new Login();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Redirect* FrameFactory::createRedirect(){
        if(pver >= 3){
            Redirect* f = new Redirect();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetFeatures* FrameFactory::createGetFeatures(){
        if(pver >= 3){
            GetFeatures* f = new GetFeatures();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    Features* FrameFactory::createFeatures(){
        if(pver >= 3){
            Features* f = new Features();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    Ping* FrameFactory::createPing(){
        if(pver >= 3){
            Ping* f = new Ping();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetObjectById* FrameFactory::createGetObjectById(){
        GetObjectById* f = new GetObjectById();
        f->setProtocolVersion(pver);
        return f;
    }
    
    GetObjectByPos* FrameFactory::createGetObjectByPos(){
        GetObjectByPos* f = new GetObjectByPos();
        f->setProtocolVersion(pver);
        return f;
    }
    
    GetObjectIdsList* FrameFactory::createGetObjectsIdsList(){
        if(pver >= 3){
            GetObjectIdsList* f = new GetObjectIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetObjectIdsByPos* FrameFactory::createGetObjectIdsByPos(){
        if(pver >= 3){
            GetObjectIdsByPos* f = new GetObjectIdsByPos();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetObjectIdsByContainer* FrameFactory::createGetObjectIdsByContainer(){
        if(pver >= 3){
            GetObjectIdsByContainer* f = new GetObjectIdsByContainer();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    ObjectIdsList* FrameFactory::createObjectIdsList(){
        if(pver >= 3){
            ObjectIdsList* f = new ObjectIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetOrderDescription* FrameFactory::createGetOrderDescription(){
        GetOrderDescription* f = new GetOrderDescription();
        f->setProtocolVersion(pver);
        return f;
    }
    
    OrderDescription* FrameFactory::createOrderDescription(){
        OrderDescription* f = new OrderDescription();
        f->setProtocolVersion(pver);
        return f;
    }
    
    GetOrderTypesList* FrameFactory::createGetOrderTypesList(){
        if(pver >= 3){
            GetOrderTypesList* f = new GetOrderTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    OrderTypesList* FrameFactory::createOrderTypesList(){
        if(pver >= 3){
            OrderTypesList* f = new OrderTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetOrder* FrameFactory::createGetOrder(){
        GetOrder* f = new GetOrder();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Order* FrameFactory::createOrder(){
        Order* f = new Order();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Order* FrameFactory::createInsertOrder(){
        return createOrder();
    }
    
    RemoveOrder* FrameFactory::createRemoveOrder(){
        RemoveOrder* f = new RemoveOrder();
        f->setProtocolVersion(pver);
        return f;
    }
    
    ProbeOrder* FrameFactory::createProbeOrder(){
        if(pver >= 3){
            ProbeOrder* f = new ProbeOrder();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetTime* FrameFactory::createGetTimeRemaining(){
        GetTime* f = new GetTime();
        f->setProtocolVersion(pver);
        return f;
    }
    
    TimeRemaining* FrameFactory::createTimeRemaining(){
        TimeRemaining* f = new TimeRemaining();
        f->setProtocolVersion(pver);
        return f;
    }
    
    GetBoard* FrameFactory::createGetBoard(){
        GetBoard* f = new GetBoard();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Board* FrameFactory::createBoard(){
        Board* f = new Board();
        f->setProtocolVersion(pver);
        return f;
    }
    
    GetBoardIdsList* FrameFactory::createGetBoardIdsList(){
        if(pver >= 3){
            GetBoardIdsList* f = new GetBoardIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    BoardIdsList* FrameFactory::createBoardIdsList(){
        if(pver >= 3){
            BoardIdsList* f = new BoardIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetMessage* FrameFactory::createGetMessage(){
        GetMessage* f = new GetMessage();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Message* FrameFactory::createMessage(){
        Message* f = new Message();
        f->setProtocolVersion(pver);
        return f;
    }
    
    Message* FrameFactory::createPostMessage(){
        return createMessage();
    }
    
    RemoveMessage* FrameFactory::createRemoveMessage(){
        RemoveMessage* f = new RemoveMessage();
        f->setProtocolVersion(pver);
        return f;
    }
    
    GetResourceDescription* FrameFactory::createGetResourceDescription(){
        GetResourceDescription* f = new GetResourceDescription();
        f->setProtocolVersion(pver);
        return f;
    }
    
    ResourceDescription* FrameFactory::createResourceDescription(){
        ResourceDescription* f = new ResourceDescription();
        f->setProtocolVersion(pver);
        return f;
    }
    
    GetResourceTypesList* FrameFactory::createGetResourceTypesList(){
        if(pver >= 3){
            GetResourceTypesList* f = new GetResourceTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    ResourceTypesList* FrameFactory::createResourceTypesList(){
        if(pver >= 3){
            ResourceTypesList* f = new ResourceTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetPlayer* FrameFactory::createGetPlayer(){
        if(pver >= 3){
            GetPlayer* f = new GetPlayer();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    Player* FrameFactory::createPlayer(){
        if(pver >= 3){
            Player* f = new Player();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetCategory* FrameFactory::createGetCategory(){
        if(pver >= 3){
            GetCategory* f = new GetCategory();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    Category* FrameFactory::createCategory(){
        if(pver >= 3){
            Category* f = new Category();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    AddCategory* FrameFactory::createAddCategory(){
        if(pver >= 3){
            AddCategory* f = new AddCategory();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    RemoveCategory* FrameFactory::createRemoveCategory(){
        if(pver >= 3){
            RemoveCategory* f = new RemoveCategory();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetCategoryIdsList* FrameFactory::createGetCategoryIdsList(){
        if(pver >= 3){
            GetCategoryIdsList* f = new GetCategoryIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    CategoryIdsList* FrameFactory::createCategoryIdsList(){
        if(pver >= 3){
            CategoryIdsList* f = new CategoryIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetDesign* FrameFactory::createGetDesign(){
        if(pver >= 3){
            GetDesign* f = new GetDesign();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    Design* FrameFactory::createDesign(){
        if(pver >= 3){
            Design* f = new Design();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    AddDesign* FrameFactory::createAddDesign(){
        if(pver >= 3){
            AddDesign* f = new AddDesign();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    ModifyDesign* FrameFactory::createModifyDesign(){
        if(pver >= 3){
            ModifyDesign* f = new ModifyDesign();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    RemoveDesign* FrameFactory::createRemoveDesign(){
        if(pver >= 3){
            RemoveDesign* f = new RemoveDesign();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetDesignIdsList* FrameFactory::createGetDesignIdsList(){
        if(pver >= 3){
            GetDesignIdsList* f = new GetDesignIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    DesignIdsList* FrameFactory::createDesignIdsList(){
        if(pver >= 3){
            DesignIdsList* f = new DesignIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetComponent* FrameFactory::createGetComponent(){
        if(pver >= 3){
            GetComponent* f = new GetComponent();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    Component* FrameFactory::createComponent(){
        if(pver >= 3){
            Component* f = new Component();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetComponentIdsList* FrameFactory::createGetComponentIdsLIst(){
        if(pver >= 3){
            GetComponentIdsList* f = new GetComponentIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    ComponentIdsList* FrameFactory::createComponentIdsList(){
        if(pver >= 3){
            ComponentIdsList* f = new ComponentIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetProperty* FrameFactory::createGetProperty(){
        if(pver >= 3){
            GetProperty* f = new GetProperty();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    Property* FrameFactory::createProperty(){
        if(pver >= 3){
            Property* f = new Property();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    GetPropertyIdsList* FrameFactory::createGetPropertyIdsList(){
        if(pver >= 3){
            GetPropertyIdsList* f = new GetPropertyIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    PropertyIdsList* FrameFactory::createPropertyIdsList(){
        if(pver >= 3){
            PropertyIdsList* f = new PropertyIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

}
