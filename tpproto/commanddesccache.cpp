/*  CommandDescCache - Cache of CommandDescriptions class
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cachemethod.h"
#include "commanddesc.h"
#include "getcommanddesc.h"
#include "getcommandtypeslist.h"
#include "protocollayer.h"
#include "framefactory.h"

#include "commanddesccache.h"

namespace TPProto {

    /*! \brief Default Constructor.
    */
    CommandDescCache::CommandDescCache() : Cache()
    {
    }

    /*! \brief Destructor.
    */
    CommandDescCache::~CommandDescCache()
    {
    }

    /*! \brief Request a command description.
        \param cmdtype The command type ID.
        \param cb The callback function for the description.
    */
    void CommandDescCache::requestCommandDescription(uint32_t cmdtype, const CommandDescCallback & cb)
    {
        CommandDescSignal * bs = waiters[cmdtype];
        if(bs == NULL){
            bs = new CommandDescSignal();
            waiters[cmdtype] = bs;
        }
        bs->connect(cb);
        cache->getById(cmdtype);
    }
    
    /*! \brief Watch a command description.
        \param cmdtype The command type ID.
        \param cb The callback function for the description.
        \return The connection for the callback.
    */
    boost::signals::connection CommandDescCache::watchCommandDescription(uint32_t cmdtype, const CommandDescCallback & cb)
    {
        CommandDescSignal * bs = watchers[cmdtype];
        if(bs == NULL){
            bs = new CommandDescSignal();
            watchers[cmdtype] = bs;
        }
        boost::signals::connection conn = bs->connect(cb);
        requestCommandDescription(cmdtype, cb);
        return conn;
    }
    
    /*! \brief Request a list of command type IDs.
        \param cb The callback function for the sequence.
    */
    void CommandDescCache::requestCommandTypes(const IdSetCallback & cb)
    {
        cache->getAllIds(cb);
    }
    
    /*! \brief Watch the list of command type IDs.
        \param cb The callback function for the sequence.
        \return The connection for the callback.
    */
    boost::signals::connection CommandDescCache::watchCommandTypes(const IdSetCallback & cb)
    {
        return cache->watchAllIds(cb);
    }
    
    /*! \brief Create a Get Command Type IDs frame.
        \return The Get Command Type IDs frame.
    */
    GetIdSequence * CommandDescCache::createGetIdSequenceFrame()
    {
        return protocol->getFrameFactory()->createGetCommandTypesList();
    }

    /*! \brief Create a Get Command Description frame.
        \return The Get Command Description frame.
    */
    GetById * CommandDescCache::createGetByIdFrame()
    {
        return protocol->getFrameFactory()->createGetCommandDescription();
    }

    /*! \brief Get the command type ID from a Command Description frame.
        \param frame The Command Description frame.
        \return The command type ID.
    */
    uint32_t CommandDescCache::getIdFromFrame(Frame* frame)
    {
        CommandDescription * cmd = dynamic_cast<CommandDescription*>(frame);
        if(cmd != NULL && cmd->getType() == ftad_CommandDesc){
            return cmd->getCommandType();
        }else{
            return 0xffffffff;
        }
    }

    /*! \brief Get the modification time from a Command Description frame.
        \param frame The Command Description frame.
        \return The modification time.
    */
    uint64_t CommandDescCache::getModTimeFromFrame(Frame* frame)
    {
        CommandDescription * cmd = dynamic_cast<CommandDescription*>(frame);
        if(cmd != NULL && cmd->getType() == ftad_CommandDesc){
            return cmd->getLastModifiedTime();
        }else{
            return 0LL;
        }
    }
  
    void CommandDescCache::newItem(boost::shared_ptr<Frame> item)
    {
        boost::shared_ptr<CommandDescription> cmddesc(boost::dynamic_pointer_cast<CommandDescription>(item));
        if(cmddesc){
            CommandDescSignal * bs = waiters[cmddesc->getCommandType()];
            if(bs != NULL){
                (*bs)(cmddesc);
                delete bs;
            }
            waiters.erase(cmddesc->getCommandType());
            bs = watchers[cmddesc->getCommandType()];
            if(bs != NULL){
                (*bs)(cmddesc);
            }
        }
        
    }
   
    void CommandDescCache::existingItem(boost::shared_ptr<Frame> item)
    {
        boost::shared_ptr<CommandDescription> cmddesc(boost::dynamic_pointer_cast<CommandDescription>(item));
        if(cmddesc){
            CommandDescSignal* bs = waiters[cmddesc->getCommandType()];
            if(bs != NULL){
                (*bs)(cmddesc);
                delete bs;
            }
            waiters.erase(cmddesc->getCommandType());
        }
    }

}
