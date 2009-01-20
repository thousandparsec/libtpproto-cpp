#ifndef TPPROTO_FEATURESFRAME_H
#define TPPROTO_FEATURESFRAME_H
/*  Features frame class
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
  \brief Declares the Features Frame.
*/

#include <set>
#include <tpproto/frame.h>

namespace TPProto{

    /*! \brief The Features frame is sent from the server to describe some of the options it supports.
    */
    class Features : public Frame{
    public:
        Features();
        virtual ~Features();

        void packBuffer(Buffer * buf);
        bool unpackBuffer(Buffer * buf);

        bool hasSecureThisPort() const;
        bool hasSecureOtherPort() const;
        bool hasHttpTunnelingThisPort() const;
        bool hasHttpTunnelingOtherPort() const;
        bool supportsPing() const;
        bool supportsServerSidePropertyCalc() const;
        bool supportsAccountCreation() const;
        bool hasFilterSSL() const;
        bool hasFilterPadding() const;

        bool isSet(uint32_t val) const;

    protected:
        /*! \brief The set of features supported by the server.
        */
        std::set<uint32_t> features;
    };

}

#endif
