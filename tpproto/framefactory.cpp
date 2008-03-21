/*  FrameFactory class
 *
 *  Copyright (C) 2005,2006, 2008  Lee Begg and the Thousand Parsec Project
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
#include "createaccount.h"
#include "login.h"
#include "redirect.h"
#include "getfeatures.h"
#include "featuresframe.h"
#include "setfilters.h"
#include "getgameinfo.h"
#include "gameinfo.h"
#include "ping.h"
#include "finished.h"
#include "getobjecttypeslist.h"
#include "objecttypeslist.h"
#include "getobjectdesc.h"
#include "objectdesc.h"
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
        - version 4 of the protocol
    */
    FrameFactory::FrameFactory(){
        pver = 4; // TP04
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

    /*! \brief Creates an OkFrame.
    \return A new OkFrame.
    */
    OkFrame* FrameFactory::createOk(){
        OkFrame* ok = new OkFrame();
        ok->setProtocolVersion(pver);
        return ok;
    }

    /*! \brief Creates a FailFrame.
    \return A new FailFrame.
    */
    FailFrame* FrameFactory::createFail(){
        FailFrame* f = new FailFrame();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a Sequence Frame.
    \return A new Sequence Frame.
    */
    Sequence* FrameFactory::createSequence(){
        Sequence* f = new Sequence();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a Connect Frame.
    \return A new Connect Frame.
    */
    Connect* FrameFactory::createConnect(){
        Connect* f = new Connect();
        f->setProtocolVersion(pver);
        return f;
    }

    /*! \brief Creates an AccountCreate Frame.
    \return A new AccountCreate Frame.
    */
    AccountCreate* FrameFactory::createAccountCreate(){
        AccountCreate* f = new AccountCreate();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a Login Frame.
    \return A new Login Frame.
    */
    Login* FrameFactory::createLogin(){
        Login* f = new Login();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a Redirect Frame.
    \return A new Redirect Frame.
    */
    Redirect* FrameFactory::createRedirect(){
        if(pver >= 3){
            Redirect* f = new Redirect();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetFeatures Frame.
    \return A new GetFeatures Frame.
    */
    GetFeatures* FrameFactory::createGetFeatures(){
        if(pver >= 3){
            GetFeatures* f = new GetFeatures();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a Features Frame.
    \return A new Features Frame.
    */
    Features* FrameFactory::createFeatures(){
        if(pver >= 3){
            Features* f = new Features();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

    /*! \brief Creates a Set Filters Frame.
    \return A new Set Filters Frame.
    */
    SetFilters* FrameFactory::createSetFilters(){
        if(pver >= 4){
            SetFilters* f = new SetFilters();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    /*! \brief Creates a Get Game Info Frame.
    \return A new Get Game Info Frame.
    */
    GetGameInfo* FrameFactory::createGetGameInfo(){
        if(pver >= 4){
            GetGameInfo* f = new GetGameInfo();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }
    
    /*! \brief Creates a Game Info Frame.
    \return A new Game Info Frame.
    */
    GameInfo* FrameFactory::createGameInfo(){
        if(pver >= 4){
            GameInfo* f = new GameInfo();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a Ping Frame.
    \return A new Ping Frame.
    */
    Ping* FrameFactory::createPing(){
        if(pver >= 3){
            Ping* f = new Ping();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

    /*! \brief Creates a Finished Turn Frame.
    \return A new Finished Turn Frame.
    */
    FinishedFrame* FrameFactory::createFinished(){
        if(pver >= 4){
            FinishedFrame* f = new FinishedFrame();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

    /*! \brief Creates a Get Object Types List Frame.
    \return A new Get Object Types List Frame.
    */
    GetObjectTypesList* FrameFactory::createGetObjectTypesList(){
        if(pver >= 4){
            GetObjectTypesList* f = new GetObjectTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

    /*! \brief Creates an Object Types List Frame.
    \return A new Object Types List Frame.
    */
    ObjectTypesList* FrameFactory::createObjectTypesList(){
        if(pver >= 4){
            ObjectTypesList* f = new ObjectTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

    /*! \brief Creates a Get Object Description Frame.
    \return A new Get Object Description Frame.
    */
    GetObjectDescription* FrameFactory::createGetObjectDescription(){
        if(pver >= 4){
            GetObjectDescription* f = new GetObjectDescription();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

    /*! \brief Creates an Object Description Frame.
    \return A new Object Description Frame.
    */
    ObjectDescription* FrameFactory::createObjectDescription(){
        if(pver >= 4){
            ObjectDescription* f = new ObjectDescription();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetObjectById Frame.
    \return A new GetObjectById Frame.
    */
    GetObjectById* FrameFactory::createGetObjectById(){
        GetObjectById* f = new GetObjectById();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a GetObjectByPos Frame.
    \return A new GetObjectByPos Frame.
    */
    GetObjectByPos* FrameFactory::createGetObjectByPos(){
        GetObjectByPos* f = new GetObjectByPos();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a GetObjectIdsList Frame.
    \return A new GetObjectIdsList Frame.
    */
    GetObjectIdsList* FrameFactory::createGetObjectIdsList(){
        if(pver >= 3){
            GetObjectIdsList* f = new GetObjectIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetObjectIdsByPos Frame.
    \return A new GetObjectIdsByPos Frame.
    */
    GetObjectIdsByPos* FrameFactory::createGetObjectIdsByPos(){
        if(pver >= 3){
            GetObjectIdsByPos* f = new GetObjectIdsByPos();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetObjectIdsByContainer Frame.
    \return A new GetObjectIdsByContainer Frame.
    */
    GetObjectIdsByContainer* FrameFactory::createGetObjectIdsByContainer(){
        if(pver >= 3){
            GetObjectIdsByContainer* f = new GetObjectIdsByContainer();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates an ObjectIdsList Frame.
    \return A new ObjectIdsList Frame.
    */
    ObjectIdsList* FrameFactory::createObjectIdsList(){
        if(pver >= 3){
            ObjectIdsList* f = new ObjectIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetOrderDescription Frame.
    \return A new GetOrderDescription Frame.
    */
    GetOrderDescription* FrameFactory::createGetOrderDescription(){
        GetOrderDescription* f = new GetOrderDescription();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates an OrderDescription Frame.
    \return A new OrderDescription Frame.
    */
    OrderDescription* FrameFactory::createOrderDescription(){
        OrderDescription* f = new OrderDescription();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a GetOrderTypesList Frame.
    \return A new GetOrderTypesList Frame.
    */
    GetOrderTypesList* FrameFactory::createGetOrderTypesList(){
        if(pver >= 3){
            GetOrderTypesList* f = new GetOrderTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates an OrderTypesList Frame.
    \return A new OrderTypesList Frame.
    */
    OrderTypesList* FrameFactory::createOrderTypesList(){
        if(pver >= 3){
            OrderTypesList* f = new OrderTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetOrder Frame.
    \return A new GetOrder Frame.
    */
    GetOrder* FrameFactory::createGetOrder(){
        GetOrder* f = new GetOrder();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates an Order Frame.
    \return A new Order Frame.
    */
    Order* FrameFactory::createOrder(){
        Order* f = new Order();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates an InsertOrder Frame.
    \return A new InsertOrder Frame.
    */
    Order* FrameFactory::createInsertOrder(){
        return createOrder();
    }

     /*! \brief Creates a RemoveOrder Frame.
    \return A new RemoveOrder Frame.
    */
    RemoveOrder* FrameFactory::createRemoveOrder(){
        RemoveOrder* f = new RemoveOrder();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a ProbeOrder Frame.
    \return A new ProbeOrder Frame.
    */
    ProbeOrder* FrameFactory::createProbeOrder(){
        if(pver >= 3){
            ProbeOrder* f = new ProbeOrder();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetTimeRemaining Frame.
    \return A new GetTimeRemaining Frame.
    */
    GetTime* FrameFactory::createGetTimeRemaining(){
        GetTime* f = new GetTime();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a TimeRemaining Frame.
    \return A new TimeRemaining Frame.
    */
    TimeRemaining* FrameFactory::createTimeRemaining(){
        TimeRemaining* f = new TimeRemaining();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a GetBoard Frame.
    \return A new GetBoard Frame.
    */
    GetBoard* FrameFactory::createGetBoard(){
        GetBoard* f = new GetBoard();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a Board Frame.
    \return A new Board Frame.
    */
    Board* FrameFactory::createBoard(){
        Board* f = new Board();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a GetBoardIdsList Frame.
    \return A new GetBoardIdsList Frame.
    */
    GetBoardIdsList* FrameFactory::createGetBoardIdsList(){
        if(pver >= 3){
            GetBoardIdsList* f = new GetBoardIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a BoardIdsList Frame.
    \return A new BoardIdsList Frame.
    */
    BoardIdsList* FrameFactory::createBoardIdsList(){
        if(pver >= 3){
            BoardIdsList* f = new BoardIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetMessage Frame.
    \return A new GetMessage Frame.
    */
    GetMessage* FrameFactory::createGetMessage(){
        GetMessage* f = new GetMessage();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a Message Frame.
    \return A new Message Frame.
    */
    Message* FrameFactory::createMessage(){
        Message* f = new Message();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a PostMessage Frame.
    \return A new PostMessage Frame.
    */
    Message* FrameFactory::createPostMessage(){
        return createMessage();
    }

     /*! \brief Creates a RemoveMessage Frame.
    \return A new RemoveMessage Frame.
    */
    RemoveMessage* FrameFactory::createRemoveMessage(){
        RemoveMessage* f = new RemoveMessage();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a GetResoureceDescription Frame.
    \return A new GetResourceDescription Frame.
    */
    GetResourceDescription* FrameFactory::createGetResourceDescription(){
        GetResourceDescription* f = new GetResourceDescription();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a ResourceDescription Frame.
    \return A new ResourceDescription Frame.
    */
    ResourceDescription* FrameFactory::createResourceDescription(){
        ResourceDescription* f = new ResourceDescription();
        f->setProtocolVersion(pver);
        return f;
    }

     /*! \brief Creates a GetResourceTypesList Frame.
    \return A new GetResourceTypesList Frame.
    */
    GetResourceTypesList* FrameFactory::createGetResourceTypesList(){
        if(pver >= 3){
            GetResourceTypesList* f = new GetResourceTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a ResourceTypesList Frame.
    \return A new ResourceTypesList Frame.
    */
    ResourceTypesList* FrameFactory::createResourceTypesList(){
        if(pver >= 3){
            ResourceTypesList* f = new ResourceTypesList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetPlayer Frame.
    \return A new GetPlayer Frame.
    */
    GetPlayer* FrameFactory::createGetPlayer(){
        if(pver >= 3){
            GetPlayer* f = new GetPlayer();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a Player Frame.
    \return A new Player Frame.
    */
    Player* FrameFactory::createPlayer(){
        if(pver >= 3){
            Player* f = new Player();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetCategory Frame.
    \return A new GetCategory Frame.
    */
    GetCategory* FrameFactory::createGetCategory(){
        if(pver >= 3){
            GetCategory* f = new GetCategory();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a Category Frame.
    \return A new Category Frame.
    */
    Category* FrameFactory::createCategory(){
        if(pver >= 3){
            Category* f = new Category();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates an AddCategory Frame.
    \return A new AddCategory Frame.
    */
    AddCategory* FrameFactory::createAddCategory(){
        if(pver >= 3){
            AddCategory* f = new AddCategory();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a RemoveCategory Frame.
    \return A new RemoveCategory Frame.
    */
    RemoveCategory* FrameFactory::createRemoveCategory(){
        if(pver >= 3){
            RemoveCategory* f = new RemoveCategory();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetCategoryIdsList Frame.
    \return A new GetCategoryIdsList Frame.
    */
    GetCategoryIdsList* FrameFactory::createGetCategoryIdsList(){
        if(pver >= 3){
            GetCategoryIdsList* f = new GetCategoryIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a CategoryIdsList Frame.
    \return A new CategoryIdsList Frame.
    */
    CategoryIdsList* FrameFactory::createCategoryIdsList(){
        if(pver >= 3){
            CategoryIdsList* f = new CategoryIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetDesign Frame.
    \return A new GetDesign Frame.
    */
    GetDesign* FrameFactory::createGetDesign(){
        if(pver >= 3){
            GetDesign* f = new GetDesign();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a Design Frame.
    \return A new Design Frame.
    */
    Design* FrameFactory::createDesign(){
        if(pver >= 3){
            Design* f = new Design();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates an AddDesign Frame.
    \return A new AddDesign Frame.
    */
    AddDesign* FrameFactory::createAddDesign(){
        if(pver >= 3){
            AddDesign* f = new AddDesign();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a ModifyDesign Frame.
    \return A new ModifyDesign Frame.
    */
    ModifyDesign* FrameFactory::createModifyDesign(){
        if(pver >= 3){
            ModifyDesign* f = new ModifyDesign();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a RemoveDesign Frame.
    \return A new RemoveDesign Frame.
    */
    RemoveDesign* FrameFactory::createRemoveDesign(){
        if(pver >= 3){
            RemoveDesign* f = new RemoveDesign();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetDesignIdsList Frame.
    \return A new GetDesignIdsList Frame.
    */
    GetDesignIdsList* FrameFactory::createGetDesignIdsList(){
        if(pver >= 3){
            GetDesignIdsList* f = new GetDesignIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a DesignIdsList Frame.
    \return A new DesignIdsList Frame.
    */
    DesignIdsList* FrameFactory::createDesignIdsList(){
        if(pver >= 3){
            DesignIdsList* f = new DesignIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetComponent Frame.
    \return A new GetComponent Frame.
    */
    GetComponent* FrameFactory::createGetComponent(){
        if(pver >= 3){
            GetComponent* f = new GetComponent();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a Component Frame.
    \return A new Component Frame.
    */
    Component* FrameFactory::createComponent(){
        if(pver >= 3){
            Component* f = new Component();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetComponentIdsList Frame.
    \return A new GetCompenentIdsList Frame.
    */
    GetComponentIdsList* FrameFactory::createGetComponentIdsList(){
        if(pver >= 3){
            GetComponentIdsList* f = new GetComponentIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a ComponentIdsList Frame.
    \return A new ComponentIdsList Frame.
    */
    ComponentIdsList* FrameFactory::createComponentIdsList(){
        if(pver >= 3){
            ComponentIdsList* f = new ComponentIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetProperty Frame.
    \return A new GetProperty Frame.
    */
    GetProperty* FrameFactory::createGetProperty(){
        if(pver >= 3){
            GetProperty* f = new GetProperty();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a Property Frame.
    \return A new Property Frame.
    */
    Property* FrameFactory::createProperty(){
        if(pver >= 3){
            Property* f = new Property();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a GetPropertyIdsList Frame.
    \return A new GetPropertyIdsList Frame.
    */
    GetPropertyIdsList* FrameFactory::createGetPropertyIdsList(){
        if(pver >= 3){
            GetPropertyIdsList* f = new GetPropertyIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

     /*! \brief Creates a PropertyIdsList Frame.
    \return A new PropertyIdsList Frame.
    */
    PropertyIdsList* FrameFactory::createPropertyIdsList(){
        if(pver >= 3){
            PropertyIdsList* f = new PropertyIdsList();
            f->setProtocolVersion(pver);
            return f;
        }
        return NULL;
    }

}
