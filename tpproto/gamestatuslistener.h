#ifndef TPPROTO_GAMESTATUSLISTENER_H
#define TPPROTO_GAMESTATUSLISTENER_H
/*  GameStatusListener - Listener for changes in status.
 *
 *  Copyright (C) 2005, 2008  Lee Begg and the Thousand Parsec Project
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
  \brief Declares the GameStatusListener baseclass interface.
*/

namespace TPProto{

    /*! \brief A Listener for listening for changes to the game status and connection.
    */
    class GameStatusListener{
    public:
        /*! \brief Required virtual destructor
        */
        virtual ~GameStatusListener(){};

        /*! \brief Called when a connection is established to a server.
        */
        virtual void connected() = 0;

        /*! \brief Called when libtpproto-cpp has been told to redirect.
        
        The client has already been disconnected from the redirecting
        server at this point. If the GameStatusListener returns false, then
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

        /*! \brief Called when the player's account is created sucessfully or not.
        \param status True if account created, false otherwise.
        */
        virtual void accountCreated(bool status) = 0;
        
        /*! \brief Called when the player is sucessfully logged in or not.
        \param status True if logged in, false otherwise.
        */
        virtual void loggedIn(bool status) = 0;

        /*! \brief Called when libtpproto-cpp notices that EOT has started.
        Noticed when a Time frame is sent that has 0 time. This could be well
        after it has actually started.
        */
        virtual void eotStarted() = 0;

        /*! \brief Called when libtpproto-cpp notices that EOT has ended.
        Noticed when a Time frame is sent that has non-zero time after EOT has started. This could be well
        after it has actually ended.
        */
        virtual void eotEnded() = 0;

        /*! \brief Called when an updated time to end of turn is received.
        Could be inacturate. Called after eotStated and eotEnded are called.
        */
        virtual void timeToEot(uint32_t timetoeot) = 0;

    };

}
#endif
