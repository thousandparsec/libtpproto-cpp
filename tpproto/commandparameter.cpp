/*  CommandParameter Classes
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

#include <sstream>

#include "buffer.h"

#include "commandparameter.h"

namespace TPProto{

    CommandParameter::CommandParameter()
    {
    }

    CommandParameter::CommandParameter(const CommandParameter &rhs)
    {
        name = rhs.name;
        description = rhs.description;
    }

    CommandParameter::~CommandParameter()
    {
    }

    std::string CommandParameter::getName()
    {
        return name;
    }

    std::string CommandParameter::getDescription()
    {
        return description;
    }

    void CommandParameter::setName(const std::string &nn)
    {
        name = nn;
    }

    void CommandParameter::setDescription(const std::string &nd)
    {
        description = nd;
    }

    StringCommandParameter::StringCommandParameter()
    {
    }

    StringCommandParameter::StringCommandParameter(const StringCommandParameter &rhs) : CommandParameter(rhs)
    {
        maxlength = rhs.maxlength;
        value = rhs.value;
    }

    StringCommandParameter::~StringCommandParameter()
    {
    }

    void StringCommandParameter::packBuffer(Buffer* buf)
    {
        buf->packInt(0);
        buf->packString(value);
    }

    bool StringCommandParameter::unpackBuffer(Buffer* buf)
    {
        maxlength = buf->unpackInt();
        value = buf->unpackString();

        return true;
    }

    CommandParameter* StringCommandParameter::clone()
    {
        return new StringCommandParameter(*this);
    }
    
    bool StringCommandParameter::setValueFromString(const std::string &nval)
    {
        value = nval;
        return true;
    }

    unsigned int StringCommandParameter::getMaxLength()
    {
        return maxlength;
    }

    std::string StringCommandParameter::getString()
    {
        return value;
    }

    void StringCommandParameter::setString(const std::string &nval)
    {
        value = nval;
    }

    IntegerCommandParameter::IntegerCommandParameter()
    {
    }

    IntegerCommandParameter::IntegerCommandParameter(const IntegerCommandParameter &rhs) : CommandParameter(rhs)
    {
        value = rhs.value;
    }

    IntegerCommandParameter::~IntegerCommandParameter()
    {
    }

    void IntegerCommandParameter::packBuffer(Buffer* buf)
    {
        buf->packInt(value);
    }

    bool IntegerCommandParameter::unpackBuffer(Buffer* buf)
    {
        value = buf->unpackInt();

        return true;
    }

    CommandParameter* IntegerCommandParameter::clone()
    {
        return new IntegerCommandParameter(*this);
    }

    bool IntegerCommandParameter::setValueFromString(const std::string &nval)
    {
        uint32_t ival;
        std::istringstream sval(nval);

        if(!(sval >> ival))
            return false;
        value = ival;
        return true;
    }

    uint32_t IntegerCommandParameter::getValue() const
    {
        return value;
    }

    void IntegerCommandParameter::setValue(uint32_t nval)
    {
        value = nval;
    }

}

