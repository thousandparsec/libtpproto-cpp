#ifndef TPPROTO_FRAMEFACTORY_H
#define TPPROTO_FRAMEFACTORY_H
/*  FrameCodec - changes network protocol to frame objects
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



/*! \file
 \brief Declares FrameFactory baseclass.

    The FrameFactory is a factory for the different types of frames that
    can be created.
*/

namespace TPProto{

    class ProtocolLayer;

    class OkFrame;
    class FailFrame;
    class Sequence;
    class Connect;
    class Login;
    class Redirect;
    class GetFeatures;
    class Features;
    class Ping;
    class GetObjectById;
    class GetObjectByPos;
    class Object;
    class GetObjectIdsList;
    class GetObjectIdsByPos;
    class GetObjectIdsByContainer;
    class ObjectIdsList;
    class GetOrderDescription;
    class OrderDescription;
    class GetOrderTypesList;
    class OrderTypesList;
    class GetOrder;
    class Order;
    class RemoveOrder;
    class ProbeOrder;
    class GetTime;
    class TimeRemaining;
    class GetBoard;
    class Board;
    class GetBoardIdsList;
    class BoardIdsList;
    class GetMessage;
    class Message;
    class RemoveMessage;
    class GetResourceDescription;
    class ResourceDescription;
    class GetResourceTypesList;
    class ResourceTypesList;
    class GetPlayer;
    class Player;
    class GetCategory;
    class Category;
    class AddCategory;
    class RemoveCategory;
    class GetCategoryIdsList;
    class CategoryIdsList;
    class GetDesign;
    class Design;
    class AddDesign;
    class ModifyDesign;
    class RemoveDesign;
    class GetDesignIdsList;
    class DesignIdsList;
    class GetComponent;
    class Component;
    class GetComponentIdsList;
    class ComponentIdsList;
    class GetProperty;
    class Property;
    class GetPropertyIdsList;
    class PropertyIdsList;

    /*! \brief FrameFactory baseclass.
    
    The FrameFactory creates frames of different types according to the
    parameters and the method called. Subclass this class if you want to
    create driviates of the various types of frames.
    */
    class FrameFactory{
    public:
        FrameFactory();
        virtual ~FrameFactory();

        void setProtocolVersion(int ver);
        int getProtocolVersion() const;
        void setProtocolLayer(ProtocolLayer* pl);

        virtual OkFrame* createOk();
        virtual FailFrame* createFail();
        virtual Sequence* createSequence();
        virtual Connect* createConnect();
        virtual Login* createLogin();
        virtual Redirect* createRedirect();
        virtual GetFeatures* createGetFeatures();
        virtual Features* createFeatures();
        virtual Ping* createPing();
        virtual GetObjectById* createGetObjectById();
        virtual GetObjectByPos* createGetObjectByPos();
        virtual GetObjectIdsList* createGetObjectIdsList();
        virtual GetObjectIdsByPos* createGetObjectIdsByPos();
        virtual GetObjectIdsByContainer* createGetObjectIdsByContainer();
        virtual ObjectIdsList* createObjectIdsList();
        virtual GetOrderDescription* createGetOrderDescription();
        virtual OrderDescription* createOrderDescription();
        virtual GetOrderTypesList* createGetOrderTypesList();
        virtual OrderTypesList* createOrderTypesList();
        virtual GetOrder* createGetOrder();
        virtual Order* createOrder();
        virtual Order* createInsertOrder();
        virtual RemoveOrder* createRemoveOrder();
        virtual ProbeOrder* createProbeOrder();
        virtual GetTime* createGetTimeRemaining();
        virtual TimeRemaining* createTimeRemaining();
        virtual GetBoard* createGetBoard();
        virtual Board* createBoard();
        virtual GetBoardIdsList* createGetBoardIdsList();
        virtual BoardIdsList* createBoardIdsList();
        virtual GetMessage* createGetMessage();
        virtual Message* createMessage();
        virtual Message* createPostMessage();
        virtual RemoveMessage* createRemoveMessage();
        virtual GetResourceDescription* createGetResourceDescription();
        virtual ResourceDescription* createResourceDescription();
        virtual GetResourceTypesList* createGetResourceTypesList();
        virtual ResourceTypesList* createResourceTypesList();
        virtual GetPlayer* createGetPlayer();
        virtual Player* createPlayer();
        virtual GetCategory* createGetCategory();
        virtual Category* createCategory();
        virtual AddCategory* createAddCategory();
        virtual RemoveCategory* createRemoveCategory();
        virtual GetCategoryIdsList* createGetCategoryIdsList();
        virtual CategoryIdsList* createCategoryIdsList();
        virtual GetDesign* createGetDesign();
        virtual Design* createDesign();
        virtual AddDesign* createAddDesign();
        virtual ModifyDesign* createModifyDesign();
        virtual RemoveDesign* createRemoveDesign();
        virtual GetDesignIdsList* createGetDesignIdsList();
        virtual DesignIdsList* createDesignIdsList();
        virtual GetComponent* createGetComponent();
        virtual Component* createComponent();
        virtual GetComponentIdsList* createGetComponentIdsList();
        virtual ComponentIdsList* createComponentIdsList();
        virtual GetProperty* createGetProperty();
        virtual Property* createProperty();
        virtual GetPropertyIdsList* createGetPropertyIdsList();
        virtual PropertyIdsList* createPropertyIdsList();

    protected:
        int pver;
        ProtocolLayer* layer;

    };

}

#endif
