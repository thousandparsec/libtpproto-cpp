#ifndef DOWNLOADPRINTVISITOR_H
#define DOWNLOADPRINTVISITOR_H

#include <tpproto/objectvisitor.h>

namespace TPProto{
  class FrameCodec;
}

class DownloadPrintVisitor : public TPProto::ObjectVisitor{
 public:
  DownloadPrintVisitor();
  virtual ~DownloadPrintVisitor();

  void visit(TPProto::Universe* ob);
  void visit(TPProto::Galaxy* ob);
  void visit(TPProto::StarSystem* ob);
  void visit(TPProto::Planet* ob);
  void visit(TPProto::Fleet* ob);
  void visit(TPProto::Object* ob);

  void setFrameCodec(TPProto::FrameCodec* nfc);
  unsigned int getPlayableObject();

 private:
  TPProto::FrameCodec * fc;
  unsigned int funobject;

};

#endif
