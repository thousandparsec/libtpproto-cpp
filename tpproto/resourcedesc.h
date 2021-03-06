#ifndef TPPROTO_RESOURCEDESC_H
#define TPPROTO_RESOURCEDESC_H
/*  ResourceDescription class
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
  \brief Declares the ResourceDescription Frame.
*/

#include <string>
#include <tpproto/frame.h>

namespace TPProto{

    /*! \brief A Frame of a Resource Description from the server.
    */
    class ResourceDescription : public Frame{
    public:
        ResourceDescription();
        virtual ~ResourceDescription();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        uint32_t getResourceType() const;
        std::string getSingularName() const;
        std::string getPluralName() const;
        std::string getSingularUnit() const;
        std::string getPluralUnit() const;
        std::string getDescription() const;
        uint32_t getWeight() const;
        uint32_t getSize() const;
        uint64_t getModTime() const;

    protected:
        /*! \brief The Resource's type number
        */
        uint32_t resid;
        /*! \brief The name of the resource in singular form.
        */
        std::string namesing;
        /*! \brief The name of the resource in plural form.
        */
        std::string nameplur;
        /*! \brief The units of the resource in signular form.
        */
        std::string unitsing;
        /*! \brief The units of the resource in plural form.
        */
        std::string unitplur;
        /*! \brief The description of the resource.
        */
        std::string desc;
        /*! \brief The mass of one unit of the resource.
        */
        uint32_t mass;
        /*! \brief The "volume" of one unit of the resource.
        */
        uint32_t size;
        /*! \brief The unix timestamp of the last time this resource description changed.
        */
        uint64_t modtime;

    };

}

#endif
