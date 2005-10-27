#ifndef TPPROTO_COMPONENT_H
#define TPPROTO_COMPONENT_H
/*  Component class
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
  \brief Declares the Component Frame.
*/

#include <string>
#include <map>
#include <set>
#include <tpproto/frame.h>

namespace TPProto{

    /*! \brief A Frame to hold Components from the server.
    */
    class Component : public Frame{
    public:
        Component();
        virtual ~Component();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        uint32_t getComponentId() const;
        uint64_t getModTime() const;
        std::set<uint32_t> getCategories() const;
        std::string getName() const;
        std::string getDescription() const;
        std::string getTpclRequirementsFunc() const;
        std::map<uint32_t, std::string> getTpclPropertyValueFuncs() const;

    private:
        uint32_t compid;
        uint64_t modtime;
        std::set<uint32_t> categories;
        std::string name;
        std::string description;
        std::string tpcl_requires;
        std::map<uint32_t, std::string> tpcl_propvalues;

    };

}

#endif
