#ifndef TPPROTO_ADMINSTATUSLISTENER_H
#define TPPROTO_ADMINSTATUSLISTENER_H
/*  AdminStatusListener - Listener for changes in status.
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
    \brief Declares the AdminStatusListener baseclass interface.
*/

namespace TPProto{

    /*! \brief A Listener for listening for changes to the connection status.
    */
    class AdminStatusListener{
      public:
        /*! \brief Required virtual destructor
        */
        virtual ~AdminStatusListener(){};

        /*! \brief Called when a connection is established to a server.
        */
        virtual void connected() = 0;

        /*! \brief Called when libtpproto-cpp has been told to redirect.
        
        The client has already been disconnected from the redirecting
        server at this point. If the AdminStatusListener returns false, then
        the client must manually do the redirect. This should be helpful for
        allowing the client to pop up a dialog asking the user if they want to
        redirect.
        \param url The url the redirection points to.
        \return True to follow the redirect, false not to.
        */
        virtual bool redirected(const std::string& url) = 0;

        /*! \brief Called when the connection is lost to the server.
        */
        virtual void disconnected() = 0;

        /*! \brief Called when the user is sucessfully logged in or not.
        \param status True if logged in, false otherwise.
        */
        virtual void loggedIn(bool status) = 0;

    };

}
#endif
