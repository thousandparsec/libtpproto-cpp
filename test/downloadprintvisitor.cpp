
#include <iostream>

#include <tpproto/object.h>
#include <tpproto/universe.h>
#include <tpproto/galaxy.h>
#include <tpproto/starsystem.h>
#include <tpproto/planet.h>
#include <tpproto/fleet.h>

#include "downloadprintvisitor.h"

using namespace TPProto;

DownloadPrintVisitor::DownloadPrintVisitor(){
}

DownloadPrintVisitor::~DownloadPrintVisitor(){
}

void DownloadPrintVisitor::visit(Universe* ob){
  std::cout << "Visiting Universe" << std::endl;

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Galaxy* ob){
  std::cout << "Visiting Galaxy" << std::endl;

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(StarSystem* ob){
   std::cout << "Visiting StarSystem" << std::endl;

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Planet* ob){
   std::cout << "Visiting Planet" << std::endl;

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Fleet* ob){
   std::cout << "Visiting Fleet" << std::endl;

  visit((Object*)ob);
}

void DownloadPrintVisitor::visit(Object* ob){

  std::cout << "Downloading Contained Objects" << std::endl;
  
 
  std::cout << "Visiting contained objects" << std::endl;

  std::cout << "Cleaning up" << std::endl;
}

void DownloadPrintVisitor::setFrameCodec(FrameCodec* nfc){
  fc = nfc;
}
