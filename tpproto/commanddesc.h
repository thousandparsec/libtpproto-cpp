#ifndef TPPROTO_COMMANDDESC_H
#define TPPROTO_COMMANDDESC_H
/*  CommandDescription Admin Frame
 *
 *  Copyright (C) 2008 Aaron Mavrinac and the Thousand Parsec Project
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
  \brief Declares CommandDescription Admin Frame.
*/

#include <string>
#include <list>
#include <tpproto/frame.h>

namespace TPProto{

    class CommandParameter;

    /*! \brief A description of a command.
    */
    class CommandDescription : public Frame{
      public:
        CommandDescription();
        virtual ~CommandDescription();

        void packBuffer(Buffer* buf);
        bool unpackBuffer(Buffer* buf);
    
        unsigned int getCommandType();
        std::string getName();
        std::string getDescription();
        std::list<CommandParameter*> getParameters();
        uint64_t getLastModifiedTime();

      private:
        int ctype;
        std::string name;
        std::string desc;
        std::list<CommandParameter*> params;
        uint64_t modtime;

    };

}

#endif
