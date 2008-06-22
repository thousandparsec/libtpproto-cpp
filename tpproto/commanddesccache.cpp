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
    
    void CommandDescCache::requestCommandTypes(const IdSetCallback & cb)
    {
        cache->getAllIds(cb);
    }
    
    boost::signals::connection CommandDescCache::watchCommandTypes(const IdSetCallback & cb)
    {
        return cache->watchAllIds(cb);
    }
    
    
    GetIdSequence * CommandDescCache::createGetIdSequenceFrame()
    {
        return protocol->getFrameFactory()->createGetCommandTypesList();
    }

    GetById * CommandDescCache::createGetByIdFrame()
    {
        return protocol->getFrameFactory()->createGetCommandDescription();
    }

    uint32_t CommandDescCache::getIdFromFrame(Frame* frame)
    {
        CommandDescription * cmd = dynamic_cast<CommandDescription*>(frame);
        if(cmd != NULL && cmd->getType() == ftad_CommandDesc){
            return cmd->getCommandType();
        }else{
            return 0xffffffff;
        }
    }

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
        boost::shared_ptr<CommandDescription> orddesc(boost::dynamic_pointer_cast<CommandDescription>(item));
        if(cmddesc){
            CommandDescSignal* bs = waiters[cmddesc->getOrderType()];
            if(bs != NULL){
                (*bs)(cmddesc);
                delete bs;
            }
            waiters.erase(orddesc->getCommandType());
        }
    }

}
