#ifndef TPPROTO_MODIFYDESIGN_H
#define TPPROTO_MODIFYDESIGN_H
/*  ModifyDesign class
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
  \brief Declares the ModifyDesign Frame.
*/

#include <string>
#include <map>
#include <set>
#include <tpproto/frame.h>

namespace TPProto{

    class Design;

    /*! \brief A Frame to Modify Designs on the server.
    */
    class ModifyDesign : public Frame{
    public:
        ModifyDesign();
        virtual ~ModifyDesign();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        void setDesignId(uint32_t did);
        //modtime set by server
        void setCategories(std::set<uint32_t>cats);
        void setName(const std::string& n);
        void setDescription(const std::string& d);
        //num in use set by server
        void setPlayerOwner(uint32_t player);
        void setComponents(std::map<uint32_t, uint32_t> comps);
        //design feedback and property values set by server

        void copyFromDesign(Design* d);

    private:
        uint32_t designid;
        std::set<uint32_t> categories;
        std::string name;
        std::string description;
        uint32_t owner;
        std::map<uint32_t, uint32_t> components;

    };

}

#endif
