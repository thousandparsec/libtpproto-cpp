#ifndef NETTEST_H
#define NETTEST_H
/*  Does a network test of the library.
 *
 *  Copyright (C) 2004, 2005, 2008  Lee Begg and the Thousand Parsec Project
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

#include <string>

namespace TPProto{
    class SimpleEventLoop;
    class GameLayer;
}

class NetTest{
    public:
        NetTest();
        ~NetTest();
        
        void setUrl(const std::string& nu);
        void setUserPass(const std::string& nu, const std::string& np);
        
        void doTest();
        
        //Parts of the test
        void connect();
        void login();
        
        int getStatus();
    
    private:
        TPProto::GameLayer* mygame;
        TPProto::SimpleEventLoop* myel;
        int status;
        std::string url;
        std::string username;
        std::string password;
};

#endif
