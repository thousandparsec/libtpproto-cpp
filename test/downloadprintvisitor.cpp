
#include <iostream>

#include <tpproto/object.h>
#include <tpproto/universe.h>
#include <tpproto/galaxy.h>
#include <tpproto/starsystem.h>
#include <tpproto/planet.h>
#include <tpproto/fleet.h>
#include <tpproto/getobjectbyid.h>
#include <tpproto/framecodec.h>
#include <tpproto/getorder.h>
#include <tpproto/order.h>

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
  std::cout << "num orders: " << ob->getNumberOrders() << std::endl;

  std::cout << "Owned by: " << ob->getOwner() << std::endl;

  std::cout << "Getting orders" << std::endl;

  GetOrder* go = fc->createGetOrderFrame();
  go->setObjectId(ob->getId());
  go->addOrderRange(0, ob->getNumberOrders());
  std::map<unsigned int, Order*> orders = fc->getOrders(go);
  for(std::map<unsigned int, Order*>::iterator itcurr = orders.begin(); itcurr != orders.end(); ++itcurr){
    std::cout << "Order: slot " << itcurr->second->getSlot() << std::endl;
    std::cout << "type: " << itcurr->second->getType() << std::endl;
    std::cout << "num turns: " << itcurr->second->getNumTurns() << std::endl;
    std::cout << "num params: " << itcurr->second->getNumParameters() << std::endl;
    
    // for each parameter...

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

  std::cout << "Owned By: " << ob->getOwner() << std::endl;
  std::cout << "Damage: " << ob->getDamage() << std::endl;
  std::cout << "ships: Scouts(" << ob->numShips(0) << "), Frigates(" << ob->numShips(1) << "), Battleships(" <<
    ob->numShips(2) << ")" << std::endl;

  std::cout << "Getting orders" << std::endl;
  GetOrder* go = fc->createGetOrderFrame();
  go->setObjectId(ob->getId());
  go->addOrderRange(0, ob->getNumberOrders());
  std::map<unsigned int, Order*> orders = fc->getOrders(go);
  for(std::map<unsigned int, Order*>::iterator itcurr = orders.begin(); itcurr != orders.end(); ++itcurr){
    std::cout << "Order: slot " << itcurr->second->getSlot() << std::endl;
    std::cout << "type: " << itcurr->second->getType() << std::endl;
    std::cout << "num turns: " << itcurr->second->getNumTurns() << std::endl;
    std::cout << "num params: " << itcurr->second->getNumParameters() << std::endl;
    
    // for each parameter...

  }

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Object* ob){

  std::cout << "Downloading Contained Objects, " << ob->getContainedObjectIds().size() << " to fetch" << std::endl;
 
  if(ob->getContainedObjectIds().size() > 0){
    
    GetObjectByID* gobi = fc->createGetObjectByIDFrame();
    gobi->addObjectIDs(ob->getContainedObjectIds());
    std::map<unsigned int, Object*> oblist = fc->getObjects(gobi);
    
    std::cout << "Visiting contained objects, " << oblist.size() << " to visit" << std::endl;
    for(std::map<unsigned int, Object*>::iterator itcurr = oblist.begin(); itcurr != oblist.end(); ++itcurr){
      (itcurr->second)->visit(this);
    }
    
    std::cout << "Cleaning up" << std::endl;
    for(std::map<unsigned int, Object*>::iterator itcurr = oblist.begin(); itcurr != oblist.end(); ++itcurr){
      delete (itcurr->second);
    }
  }
}

void DownloadPrintVisitor::setFrameCodec(FrameCodec* nfc){
  fc = nfc;
}
