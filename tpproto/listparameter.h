#ifndef TPPROTO_LISTPARAMETER_H
#define TPPROTO_LISTPARAMETER_H

#include <map>
#include <tpproto/orderparameter.h>

namespace TPProto{

  typedef std::pair<std::string, unsigned int> SelectableElement;
  
  class ListParameter : public OrderParameter{
  public:
    ListParameter();
    ListParameter(const ListParameter& rhs);
    virtual ~ListParameter();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    OrderParameter* clone();
    void visit(OrderParameterVisitor* opv);

    bool setSelection(unsigned int id, unsigned int num);
    unsigned int getSelection(unsigned int id);

    std::map<unsigned int, SelectableElement > getSelectableList();

  private:
    std::map<unsigned int, unsigned int> chosen;
    std::map<unsigned int, SelectableElement > selectable;
  };

}

#endif
