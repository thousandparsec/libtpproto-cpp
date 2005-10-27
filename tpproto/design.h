#ifndef TPPROTO_DESIGN_H
#define TPPROTO_DESIGN_H
/*  Design class
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
  \brief Declares the Design Frame.
*/

#include <string>
#include <map>
#include <set>
#include <tpproto/frame.h>

namespace TPProto{

    /*! \brief A Frame to hold Designs from the server.
    */
    class Design : public Frame{
    public:
        Design();
        virtual ~Design();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        uint32_t getDesignId() const;
        uint64_t getModTime() const;
        std::set<uint32_t> getCategories() const;
        std::string getName() const;
        std::string getDescription() const;
        uint32_t getNumInUse() const;
        uint32_t getPlayerOwner() const;
        std::map<uint32_t, uint32_t> getComponents() const;
        std::string getDesignFeedback() const;
        std::map<uint32_t, std::string> getPropertyValues() const;

    private:
        uint32_t designid;
        uint64_t modtime;
        std::set<uint32_t> categories;
        std::string name;
        std::string description;
        uint32_t inuse;
        uint32_t owner;
        std::map<uint32_t, uint32_t> components;
        std::string feedback;
        std::map<uint32_t, std::string> propvalues;

    };

}

#endif
