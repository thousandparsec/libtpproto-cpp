#ifndef DOWNLOADPRINTVISITOR_H
#define DOWNLOADPRINTVISITOR_H

#include <tpproto/objectvisitor.h>
#include <tpproto/orderparametervisitor.h>

namespace TPProto{
  class GameLayer;
}

class DownloadPrintVisitor : public TPProto::ObjectVisitor, TPProto::OrderParameterVisitor{
 public:
  DownloadPrintVisitor();
  virtual ~DownloadPrintVisitor();

  void visit(TPProto::Universe* ob);
  void visit(TPProto::Galaxy* ob);
  void visit(TPProto::StarSystem* ob);
  void visit(TPProto::Planet* ob);
  void visit(TPProto::Fleet* ob);
  void visit(TPProto::Object* ob);

  void setGameLayer(TPProto::GameLayer* nfc);
  unsigned int getPlayableObject();

  void visitOrderParam(TPProto::SpaceCoordinates* op);
  void visitOrderParam(TPProto::TimeParameter* op);
  void visitOrderParam(TPProto::ObjectParameter* op);
  void visitOrderParam(TPProto::ListParameter* op);
  void visitOrderParam(TPProto::PlayerParameter* op);
  void visitOrderParam(TPProto::SpaceCoordinatesRel* op);
  void visitOrderParam(TPProto::RangeParameter* op);
  void visitOrderParam(TPProto::StringParameter* op);
  void printOParam(TPProto::OrderParameter* op);

 private:
  TPProto::GameLayer * fc;
  unsigned int funobject;

};

#endif
