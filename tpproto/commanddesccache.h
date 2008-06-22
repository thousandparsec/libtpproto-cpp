#ifndef TPPROTO_COMMANDDESCCACHE_H
#define TPPROTO_COMMANDDESCCACHE_H
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

/*! \file
  \brief Declares the CommandDescCache class.
*/

#include <map>
#include <boost/signal.hpp>

#include "cache.h"

namespace TPProto{

    class CommandDescription;
    
    typedef boost::signal<void (boost::shared_ptr<CommandDescription>)> CommandDescSignal;
    typedef CommandDescSignal::slot_type CommandDescCallback;

    /*! \brief A Cache that caches OrderDescriptions.
    
    */
    class CommandDescCache : public Cache{
      public:
        CommandDescCache();
        virtual ~CommandDescCache();
    
        void requestCommandDescription(uint32_t cmdtype, const CommandDescCallback &cb);
        boost::signals::connection watchCommandDescription(uint32_t cmdtype, const CommandDescCallback &cb);
    
        void requestCommandTypes(const IdSetCallback& cb);
        boost::signals::connection watchCommandTypes(const IdSetCallback& cb);

        virtual GetIdSequence* createGetIdSequenceFrame();
        virtual GetById* createGetByIdFrame();
        virtual uint32_t getIdFromFrame(Frame* frame);
        virtual uint64_t getModTimeFromFrame(Frame* frame);

        virtual void newItem(boost::shared_ptr<Frame> item);
        virtual void existingItem(boost::shared_ptr<Frame> item);

      private:
        std::map<uint32_t, CommandDescSignal*> watchers;
        std::map<uint32_t, CommandDescSignal*> waiters;
    };

}

#endif
