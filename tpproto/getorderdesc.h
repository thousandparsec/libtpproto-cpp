#ifndef TPPROTO_GETORDERDESC_H
#define TPPROTO_GETORDERDESC_H

/*! \file
  \brief Declares GetOrderDescription Frame.
*/

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

  /*! A Frame for getting OrderDescriptions.
   */
  class GetOrderDescription : public Frame{
  public:
    GetOrderDescription();
    virtual ~GetOrderDescription();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    void addOrderType(unsigned int id);
    void addOrderTypes(std::set<unsigned int> ids);
    
  private:
    std::set<unsigned int> idset;

  };

}

#endif
