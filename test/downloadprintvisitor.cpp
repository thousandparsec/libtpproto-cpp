
#include <iostream>

#include <tpproto/object.h>
#include <tpproto/universe.h>
#include <tpproto/galaxy.h>
#include <tpproto/starsystem.h>
#include <tpproto/planet.h>
#include <tpproto/fleet.h>

#include <tpproto/order.h>
#include <tpproto/orderparameter.h>
#include <tpproto/spacecoord.h>
#include <tpproto/timeparameter.h>
#include <tpproto/objectparameter.h>
#include <tpproto/listparameter.h>
#include <tpproto/playerparameter.h>
#include <tpproto/spacecoordrel.h>
#include <tpproto/rangeparameter.h>
#include <tpproto/stringparameter.h>

#include <tpproto/gamelayer.h>
#include <tpproto/player.h>
#include <tpproto/resourcedesc.h>
#include <tpproto/design.h>

#include "downloadprintvisitor.h"

using namespace TPProto;

DownloadPrintVisitor::DownloadPrintVisitor(){
}

DownloadPrintVisitor::~DownloadPrintVisitor(){
}

void DownloadPrintVisitor::visit(Universe* ob){
  std::cout << "Visiting Universe" << std::endl;
  
  std::cout << "ID: " << ob->getId() << std::endl;
  std::cout << "name: " << ob->getName() << std::endl;
  std::cout << "type: " << ob->getObjectType() << std::endl;
  std::cout << "size: " << ob->getSize() << std::endl;
  std::cout << "age: " << ob->getAge() << std::endl;

  std::set<unsigned int> obset = ob->getContainedObjectIds();
  std::cout << "Num contained: " << obset.size() << std::endl << "Contains: ";
  for(std::set<unsigned int>::iterator itcurr = obset.begin(); itcurr != obset.end(); ++itcurr){
    std::cout << (*itcurr) << " ";
  }
  std::cout << std::endl;

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Galaxy* ob){
  std::cout << "Visiting Galaxy" << std::endl;

  std::cout << "ID: " << ob->getId() << std::endl;
  std::cout << "name: " << ob->getName() << std::endl;
  std::cout << "type: " << ob->getObjectType() << std::endl;
  std::cout << "size: " << ob->getSize() << std::endl;

  std::set<unsigned int> obset = ob->getContainedObjectIds();
  std::cout << "Num contained: " << obset.size() << std::endl << "Contains: ";
  for(std::set<unsigned int>::iterator itcurr = obset.begin(); itcurr != obset.end(); ++itcurr){
    std::cout << (*itcurr) << " ";
  }
  std::cout << std::endl;

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(StarSystem* ob){
  std::cout << "Visiting StarSystem" << std::endl;

  std::cout << "ID: " << ob->getId() << std::endl;
  std::cout << "name: " << ob->getName() << std::endl;
  std::cout << "type: " << ob->getObjectType() << std::endl;
  std::cout << "size: " << ob->getSize() << std::endl;

  std::set<unsigned int> obset = ob->getContainedObjectIds();
  std::cout << "Num contained: " << obset.size() << std::endl << "Contains: ";
  for(std::set<unsigned int>::iterator itcurr = obset.begin(); itcurr != obset.end(); ++itcurr){
    std::cout << (*itcurr) << " ";
  }
  std::cout << std::endl;

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Planet* ob){
  std::cout << "Visiting Planet" << std::endl;

  std::cout << "ID: " << ob->getId() << std::endl;
  std::cout << "name: " << ob->getName() << std::endl;
  std::cout << "type: " << ob->getObjectType() << std::endl;
  std::cout << "size: " << ob->getSize() << std::endl;

  std::set<unsigned int> obset = ob->getContainedObjectIds();
  std::cout << "Num contained: " << obset.size() << std::endl << "Contains: ";
  for(std::set<unsigned int>::iterator itcurr = obset.begin(); itcurr != obset.end(); ++itcurr){
    std::cout << (*itcurr) << " ";
  }
  std::cout << std::endl;

    std::cout << "Owned by: (" << ob->getOwner() << ") ";
    Player* player = fc->getPlayer(ob->getOwner());
    if(player != NULL){
        std::cout << player->getPlayerName() << " [" << player->getRaceName() << "]";
        delete player;
    }
    std::cout << std::endl;

    std::map<uint32_t, std::pair<uint32_t, uint32_t> > res = ob->getResources();
    std::cout << "Resources (" << res.size() << "): ";
    for(std::map<uint32_t, std::pair<uint32_t, uint32_t> >::iterator itcurr = res.begin();
            itcurr != res.end(); ++itcurr){
        uint32_t resid = itcurr->first;
        ResourceDescription* resdesc = fc->getResourceDescription(resid);
        if(itcurr->second.first != 1 && itcurr->second.second != 1){
            std::cout << resdesc->getPluralName();
        }else{
            std::cout << resdesc->getSingularName();
        }
        std::cout << " (" << resid << ") on surface " << itcurr->second.first << " ";
        if(itcurr->second.first != 1)
            std::cout << resdesc->getPluralUnit();
        else
            std::cout << resdesc->getSingularUnit();
        std::cout << ", mineable " << itcurr->second.second << " ";
        if(itcurr->second.second != 1)
            std::cout << resdesc->getPluralUnit();
        else
            std::cout << resdesc->getSingularUnit();
        std::cout << "; ";
        delete resdesc;
    }
    std::cout << std::endl;

    std::cout << "num orders: " << ob->getNumberOrders() << std::endl;

  if(!ob->getAvailableOrders().empty()){
    funobject = ob->getId();
    if(ob->getNumberOrders() > 0){
      std::cout << "Getting orders" << std::endl;
      std::list<Order*> orders = fc->getOrders(ob->getId(), ob->getNumberOrders());
      for(std::list<Order*>::iterator itcurr = orders.begin(); itcurr != orders.end(); ++itcurr){
	
	std::cout << "Order: slot " << (*itcurr)->getSlot() << std::endl;
	std::cout << "type: " << (*itcurr)->getOrderType() << std::endl;
	std::cout << "num turns: " << (*itcurr)->getNumTurns() << std::endl;
	std::cout << "num params: " << (*itcurr)->getNumParameters() << std::endl;
	
	// for each parameter...
	for(unsigned int i = 0; i < (*itcurr)->getNumParameters(); i++){
	  (*itcurr)->getParameter(i)->visit(this);
	}
            delete (*itcurr);
      }
    }
  }

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Fleet* ob){
  std::cout << "Visiting Fleet" << std::endl;
  
  std::cout << "ID: " << ob->getId() << std::endl;
  std::cout << "name: " << ob->getName() << std::endl;
  std::cout << "type: " << ob->getObjectType() << std::endl;
  std::cout << "size: " << ob->getSize() << std::endl;
  std::cout << "num orders: " << ob->getNumberOrders() << std::endl;

    std::cout << "Owned by: (" << ob->getOwner() << ") ";
    Player* player = fc->getPlayer(ob->getOwner());
    if(player != NULL){
        std::cout << player->getPlayerName() << " [" << player->getRaceName() << "]";
        delete player;
    }
    std::cout << std::endl;
  std::cout << "Damage: " << ob->getDamage() << std::endl;
    std::map<int32_t, int32_t> ships = ob->getShips();
    std::cout << "Ships ( " << ships.size() << "): ";
    for(std::map<int32_t, int32_t>::iterator itcurr = ships.begin(); itcurr != ships.end(); ++itcurr){
        Design* design = fc->getDesign(itcurr->first);
        std::cout << itcurr->second << " ships of ";
        if(design != NULL){
            std::cout << design->getName();
            delete design;
        }else{
            std::cout << "unknown (" << itcurr->first << ")";
        }
        std::cout << " design, ";
    }
    std::cout << std::endl;

  if(!ob->getAvailableOrders().empty()){
    funobject = ob->getId();
    if(ob->getNumberOrders() > 0){
      std::cout << "Getting orders" << std::endl;
      std::list<Order*> orders = fc->getOrders(ob->getId(), ob->getNumberOrders());
      for(std::list<Order*>::iterator itcurr = orders.begin(); itcurr != orders.end(); ++itcurr){
	
	std::cout << "Order: slot " << (*itcurr)->getSlot() << std::endl;
	std::cout << "type: " << (*itcurr)->getOrderType() << std::endl;
	std::cout << "num turns: " << (*itcurr)->getNumTurns() << std::endl;
	std::cout << "num params: " << (*itcurr)->getNumParameters() << std::endl;
	
	// for each parameter...
	for(unsigned int i = 0; i < (*itcurr)->getNumParameters(); i++){
	  (*itcurr)->getParameter(i)->visit(this);
	}
            delete (*itcurr);
      }
    }
  }

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Object* ob){

  std::cout << "Downloading Contained Objects, " << ob->getContainedObjectIds().size() << " to fetch" << std::endl;
 
  if(ob->getContainedObjectIds().size() > 0){
    
    std::set<uint32_t> oblist = ob->getContainedObjectIds();
    std::cout << "Visiting contained objects, " << oblist.size() << " to visit" << std::endl;
    for(std::set<uint32_t>::iterator itcurr = oblist.begin(); itcurr != oblist.end(); ++itcurr){
        Object* newob = fc->getObject(*itcurr);
        newob->visit(this);
        delete newob;
    }
  }
}

void DownloadPrintVisitor::setGameLayer(GameLayer* nfc){
  fc = nfc;
}

unsigned int DownloadPrintVisitor::getPlayableObject(){
  return funobject;
}

void DownloadPrintVisitor::visitOrderParam(SpaceCoordinates* op){
  printOParam((OrderParameter*)op);
  std::cout << "Pos: <" << op->getPos().getX() << ", " << op->getPos().getY() << ", " << op->getPos().getZ() << ">" << std::endl;
}

void DownloadPrintVisitor::visitOrderParam(TimeParameter* op){
  printOParam((OrderParameter*)op);
  std::cout << "Time max: " << op->getMaximumTurns() << std::endl;
  std::cout << "Turns: " << op->getTurns() << std::endl;
}

void DownloadPrintVisitor::visitOrderParam(ObjectParameter* op){
  printOParam((OrderParameter*)op);
  std::cout << "Objectid: " << op->getObjectId() << std::endl;
}

void DownloadPrintVisitor::visitOrderParam(ListParameter* op){
  printOParam((OrderParameter*)op);
  std::cout << "List parameter object and I'm lazy" << std::endl;
}

void DownloadPrintVisitor::visitOrderParam(TPProto::PlayerParameter* op){
  printOParam((OrderParameter*)op);
  std::cout << "PlayerId: " << op->getPlayerId() << std::endl;
}

void DownloadPrintVisitor::visitOrderParam(TPProto::SpaceCoordinatesRel* op){
  printOParam((OrderParameter*)op);
  std::cout << "Object Id: " << op->getObjectId() << std::endl;
  std::cout << "Pos: <" << op->getPos().getX() << ", " << op->getPos().getY() << ", " << op->getPos().getZ() << ">" << std::endl;
}

void DownloadPrintVisitor::visitOrderParam(TPProto::RangeParameter* op){
  printOParam((OrderParameter*)op);
  std::cout << "Value: " << op->getValue() << std::endl;
  std::cout << "Limits: " << op->getMinimum() << " to " << op->getMaximum() << std::endl;
  std::cout << "Increment: " << op->getIncrement() << std::endl;
}

void DownloadPrintVisitor::visitOrderParam(TPProto::StringParameter* op){
  printOParam((OrderParameter*)op);
  std::cout << "Max length: " << op->getMaxLength() << std::endl;
  std::cout << "String: " << op->getString() << std::endl;
}

void DownloadPrintVisitor::printOParam(OrderParameter* op){
  std::cout << "OrderParameter: Name: " << op->getName() << std::endl;
  std::cout << "Description: " << op->getDescription() << std::endl;
}
