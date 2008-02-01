#ifndef TPPROTO_GAMEINFO_H
#define TPPROTO_GAMEINFO_H
/*  GameInfo - Frame from the server describing a game
 *
 *  Copyright (C) 2008  Lee Begg and the Thousand Parsec Project
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
  \brief Declares GameInfo Frame.
*/

#include <stdint.h>
#include <set>
#include <string>
#include <map>

#include <tpproto/frame.h>

namespace TPProto{

  /*! \brief A Frame for describing a game on the server.
   */
  class GameInfo : public Frame{
  public:
    GameInfo();
    virtual ~GameInfo();

    void packBuffer(Buffer* buf);
    bool unpackBuffer(Buffer* buf);

    std::string getGameName() const;
    std::set<std::string> getAvailableProtocolVersions() const;
    std::string getServerName() const;
    std::string getServerVersion() const;
    std::string getRulesetName() const;
    std::string getRulesetVersion() const;
    std::string getMediaBaseUrl() const;
    std::set<std::string> getConnectionUrls() const;
    std::string getAdvertisedIp(const std::string& url) const;
    std::string getOptionString(uint32_t optionid) const;
    uint32_t getOptionInt(uint32_t optionid) const;
    

  private:
    std::string game_name;
    std::set<std::string> pversions;
    std::string server_ver;
    std::string server_name;
    std::string rules_ver;
    std::string rules_name;
    std::map<std::string, std::string> urls;
    std::map<uint32_t, std::pair<std::string, uint32_t> > options;
    std::string media_url;
  };

}

#endif
