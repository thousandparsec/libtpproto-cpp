

#include "buffer.h"

#include "listparameter.h"

namespace TPProto{

  ListParameter::ListParameter(){
  }

  ListParameter::ListParameter(const ListParameter& rhs) : OrderParameter(rhs){
    chosen.clear();
    copy(rhs.selectable.begin(), rhs.selectable.end(), inserter(selectable, selectable.begin()));
  }

  ListParameter::~ListParameter(){
    chosen.clear();
    selectable.clear();
  }

  void ListParameter::packBuffer(Buffer* buf){
    buf->packInt(0); // selectable list
    buf->packInt(chosen.size());
    for(std::map<unsigned int, unsigned int>::iterator itcurr = chosen.begin(); itcurr != chosen.end(); 
	++itcurr){
      buf->packInt(itcurr->first);
      buf->packInt(itcurr->second);
    }
  }

  bool ListParameter::unpackBuffer(Buffer* buf){
    int numsa = buf->unpackInt();
    for(int i = 0; i < numsa; i++){
      unsigned int id = buf->unpackInt();
      char* iname = buf->unpackString();
      unsigned int maxnum = buf->unpackInt();
      selectable[id] = SelectableElement(iname, maxnum);
      delete iname;
    }
    int nums = buf->unpackInt();
    for(int i = 0; i < nums; i++){
      int id = buf->unpackInt();
      int num = buf->unpackInt();
      chosen[id] = num;
    }

    return true;
  }

  OrderParameter* ListParameter::clone(){
    return new ListParameter(*this);
  }

  bool ListParameter::setSelection(unsigned int id, unsigned int num){
    std::map<unsigned int, SelectableElement>::iterator itcurr = selectable.find(id);
    if(itcurr != selectable.end() && num <= (itcurr->second.second)){
      chosen[id] = num;
      return true;
    }
    return false;
  }

  unsigned int ListParameter::getSelection(unsigned int id){
    return chosen[id];
  }

  std::map<unsigned int, SelectableElement> ListParameter::getSelectableList(){
    return selectable;
  }

}

