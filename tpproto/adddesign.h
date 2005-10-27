#ifndef TPPROTO_ADDDESIGN_H
#define TPPROTO_ADDDESIGN_H
/*  AddDesign class
 *
 *  Copyright (C) 2005  Lee Begg and the Thousand Parsec Project
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
  \brief Declares the AddDesign Frame.
*/

#include <string>
#include <map>
#include <set>
#include <tpproto/frame.h>

namespace TPProto{

    /*! \brief A Frame to hold AddDesigns frames to send to the server.
    */
    class AddDesign : public Frame{
    public:
        AddDesign();
        virtual ~AddDesign();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        //id and modtime set by server
        void setCategories(std::set<uint32_t> cats);
        void setName(const std::string& n);
        void setDescription(const std::string& d);
        //num in use is set by server
        void setPlayerOwner(uint32_t player);
        void setComponents(std::map<uint32_t, uint32_t> comps);
        //design feeback and property values set by server

    private:
        std::set<uint32_t> categories;
        std::string name;
        std::string description;
        uint32_t owner;
        std::map<uint32_t, uint32_t> components;

    };

}

#endif
