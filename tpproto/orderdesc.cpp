
#include <cassert>

#include "buffer.h"
#include "orderparameter.h"
//order parameters
#include "spacecoord.h"
#include "timeparameter.h"
#include "objectparameter.h"
#include "listparameter.h"
#include "spacecoordrel.h"
#include "rangeparameter.h"
#include "stringparameter.h"

#include "orderdesc.h"

namespace TPProto{

  OrderDescription::OrderDescription(){
  }

  OrderDescription::~OrderDescription(){
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      delete *itcurr;
    }
    params.clear();
  }

  void OrderDescription::packBuffer(Buffer* buf){
    assert(0);
  }

  bool OrderDescription::unpackBuffer(Buffer* buf){
    otype = buf->unpackInt();
    char* temp = buf->unpackString();
    name = temp;
    delete temp;
    temp = buf->unpackString();
    desc = temp;
    delete temp;

    //unpack args
    int numparams = buf->unpackInt();

    for(int i = 0; i < numparams; i++){
      temp = buf->unpackString();
      int ptype = buf->unpackInt();
      OrderParameter* opm = NULL;
      switch(ptype){
      case opT_Space_Coord_Abs:
	opm = new SpaceCoordinates();
	break;
      case opT_Time:
	opm = new TimeParameter();
	break;
      case opT_Object_ID:
	opm = new ObjectParameter();
	break;
      case opT_List:
	opm = new ListParameter();
	break;

      default:
	break;
      }
      if(opm != NULL){
	opm->setName(temp);
      }
      delete temp;

      temp = buf->unpackString();
      if(opm != NULL){
	opm->setDescription(temp);
	params.push_back(opm);
      }
      delete temp;
    }
    
    type = ft02_OrderDesc;

    return true;
  }

  unsigned int OrderDescription::getOrderType(){
    return otype;
  }

  std::string OrderDescription::getName(){
    return name;
  }

  std::string OrderDescription::getDescription(){
    return desc;
  }

  std::list<OrderParameter*> OrderDescription::getParameters(){
    std::list<OrderParameter*> np;
    for(std::list<OrderParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
      np.push_back((*itcurr)->clone());
    }
    return np;
  }

}

