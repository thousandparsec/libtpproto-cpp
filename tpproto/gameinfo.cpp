/*  GameInfo - Info about a game on the server
 *
 *  Copyright (C) 2008 Lee Begg and the Thousand Parsec Project
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

#include <cassert>
#include <sstream>

#include "buffer.h"

#include "gameinfo.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  GameInfo::GameInfo() : Frame(), game_name(), pversions(), server_ver(), server_name(),
                     rules_ver(), rules_name(), urls(), options(), media_url(){
  }

  /*! \brief Required virtual destructor.
   */
  GameInfo::~GameInfo(){
  }

  /*! \brief Pack this GameInfo Frame into a Buffer, asserts.
  
  Packs this GameInfo frame into the buffer. Since the frame only
  comes from the server, this method assert(0).
    \param buf The Buffer to pack into, ignored.
  */
  void GameInfo::packBuffer(Buffer* buf){
    assert(0);
  }

  /*! \brief Unpack from a Buffer.
    
  Unpacks the GameInfo frame from the Buffer, if possible.
  \param buf The Buffer to unpack..
  \return True if successfully unpacked, false otherwise.
  */
  bool GameInfo::unpackBuffer(Buffer* buf){
    game_name = buf->unpackString();
    buf->unpackString(); //unpack key, should be empty
    uint32_t num = buf->unpackInt();
    for(uint32_t i =0; i < num; i++){
      pversions.insert(buf->unpackString());
    }
    server_ver = buf->unpackString();
    server_name = buf->unpackString();
    rules_name = buf->unpackString();
    rules_ver = buf->unpackString();
    num = buf->unpackInt();
    for(uint32_t i = 0; i < num; i++){
      std::string proto = buf->unpackString();
      std::string host = buf->unpackString();
      std::string ip = buf->unpackString();
      uint32_t port = buf->unpackInt();
      std::ostringstream formatter;
      formatter << proto << "://" << host;
      if((port != 6923 && proto == "tp") || (port != 6924 && proto == "tps")){
        formatter << ":" << port;
      }
      std::string url = formatter.str();
      urls[url] = ip;
    }
    //options
    num = buf->unpackInt();
    for(uint32_t i = 0; i < num; i++){
      uint32_t opid = buf->unpackInt();
      std::string opstr = buf->unpackString();
      uint32_t opval = buf->unpackInt();
      options[opid] = std::pair<std::string, uint32_t>(opstr, opval);
    }
    media_url = buf->unpackString();
    
    type = ft04_GameInfo;
    return true;
  }

  /*! \brief Gets the game name for the game.
  \return A string containing the game name.
  */
  std::string GameInfo::getGameName() const{
    return game_name;
  }

  /*! \brief Gets the available protocol versions.
  \return A set of strings, each is an available protocol version.
  */
  std::set<std::string> GameInfo::getAvailableProtocolVersions() const{
    return pversions;
  }

  /*! \brief Get the name of the server software.
  \return A string of the server software's name.
  */
  std::string GameInfo::getServerName() const{
    return server_name;
  }

  /*! \brief Get the version of the server software.
  \return A string of the server software's version.
  */
  std::string GameInfo::getServerVersion() const{
    return server_ver;
  }

  /*! \brief Get the name of the ruleset.
  \return A string of the ruleset's name.
  */
  std::string GameInfo::getRulesetName() const{
    return rules_name;
  }

  /*! \brief Get the version of the ruleset.
  \return A string of the ruleset's version.
  */
  std::string GameInfo::getRulesetVersion() const{
    return rules_ver;
  }

  /*! \brief Get the base url for media for the game.
  \return A string of the base media URL.
  */
  std::string GameInfo::getMediaBaseUrl() const{
    return media_url;
  }

  /*! \brief Get the urls this game can be connected by.
  \return A set of strings of urls
  */
  std::set<std::string> GameInfo::getConnectionUrls() const{
    std::set<std::string> rtv;
    for(std::map<std::string, std::string>::const_iterator itcurr = urls.begin();
        itcurr != urls.end(); ++itcurr){
      rtv.insert(itcurr->first);
    }
    return rtv;
  }

  /*! \brief Gets the IP address advertised for a Connection URL.
  \param url The URL to get the corresponding IP of.
  \return A string of the corresponding advertised IP address, or an empty String.
  */
  std::string GameInfo::getAdvertisedIp(const std::string& url) const{
    std::map<std::string, std::string>::const_iterator itcurr = urls.find(url);
    if(itcurr != urls.end()){
      return itcurr->second;
    }else{
      return std::string();
    }
  }

  /*! \brief Get the OptionString associated with a given option id.
  
  An empty return string could be either empty in the option, or the
  option does not exist for this game.
  \param optionid The option id to get the string of.
  \return The string of the option value, or an empty string.
  */
  std::string GameInfo::getOptionString(uint32_t optionid) const{
    std::map<uint32_t, std::pair<std::string, uint32_t> >::const_iterator itcurr = options.find(optionid);
    if(itcurr != options.end()){
      return itcurr->second.first;
    }else{
      return std::string();
    }
  }
  
  /*! \brief Get the Option Integer associated with a given option id.
  
  A value of 0 could be either 0 for the option, or the option does not exist
  for this game.
  \param optionid The option id to get the integer of.
  \return The uint32_t value of the option, or 0.
  */
  uint32_t GameInfo::getOptionInt(uint32_t optionid) const{
    std::map<uint32_t, std::pair<std::string, uint32_t> >::const_iterator itcurr = options.find(optionid);
    if(itcurr != options.end()){
      return itcurr->second.second;
    }else{
      return 0;
    }
  }

}

