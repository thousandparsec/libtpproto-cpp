/*  SetFilter - tells server what filters are requested
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


#include "buffer.h"

#include "setfilters.h"

namespace TPProto{

  /*! \brief Default constructor.
   */
  SetFilters::SetFilters() : Frame(), filters(){
  }

  /*! \brief Required virtual destructor.
   */
  SetFilters::~SetFilters(){
  }

  /*! \brief Pack this SetFilters Frame into a Buffer.
    \param buf The Buffer to pack into.
  */
  void SetFilters::packBuffer(Buffer* buf){
    type = ft04_Filters_Set;
    buf->packInt(filters.size());
    for(std::set<uint32_t>::iterator itcurr = filters.begin(); itcurr != filters.end(); ++itcurr){
      buf->packInt(*itcurr);
    }
  }

  /*! \brief Unpack from a Buffer, always false.
    
  Since SetFilters frames are only sent to the server, we should not 
  receive them and therefore this method always returns false.
  \param buf The Buffer, ignored.
  \return False always.
  */
  bool SetFilters::unpackBuffer(Buffer* buf){
    return false;
  }

  /*! \brief Adds a filter to be requested.
  
  Adds the given filter Id (which is the feature Id in the Feature frame) 
  to the set to be requested.
  \param fid The Filter Id to be requested.
  */
  void SetFilters::addFilter(uint32_t fid){
    filters.insert(fid);
  }

  /*! \brief Removes a filter to be requested.
  
  Removes the given filter id from the set to be requested.
  \param fid The filter to not be requested.
  */
  void SetFilters::removeFilter(uint32_t fid){
    filters.erase(fid);
  }

  /*! \brief Gets the set of filters to be requested.
  
  \return The set of filter Ids to be requested.
  */
  std::set<uint32_t> SetFilters::getFilters() const{
    return filters;
  }

  /*! \brief Checks if a filter is to be requested.
  
  \param fid The filter Id to check
  \return True if fid is in the set of filter Ids to request.
  */
  bool SetFilters::isSet(uint32_t fid) const{
    return (filters.count(fid) != 0);
  }

}

