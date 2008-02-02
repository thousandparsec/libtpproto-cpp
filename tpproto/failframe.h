#ifndef TPPROTO_FAILFRAME_H
#define TPPROTO_FAILFRAME_H
/*  FailFrame - Frame send by the server to tell the client that an operation failed.
 *
 *  Copyright (C) 2005, 2006, 2008  Lee Begg and the Thousand Parsec Project
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
  \brief Declares FailFrame class.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{
  
  /*! \brief Represenation of fail or error Frame from the server.
   */
  class FailFrame : public Frame{
  public:
    FailFrame();
    virtual ~FailFrame();

    void packBuffer(Buffer * buf);
    bool unpackBuffer(Buffer * buf);
    
    int getErrorCode();
    std::string getErrorString();

  private:
    int errcode;
    std::string errstring;

  };

}

#endif
