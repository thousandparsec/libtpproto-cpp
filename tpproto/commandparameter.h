#ifndef TPPROTO_COMMANDPARAMETER_H
#define TPPROTO_COMMANDPARAMETER_H
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

/*! \file
    \brief Declares CommandParameter classes and the CommandParamType enum.
*/

#include <string>
#include <stdint.h>

namespace TPProto{

    enum CommandParamType{
        cpT_Invalid = -1,
        cpT_String = 0,
        cpT_Integer = 1,
        cpT_Max
    };

    class Buffer;

    /*! \brief Baseclass for holding a parameter for a Command.
    */
    class CommandParameter{
      public:
        CommandParameter();
        CommandParameter(const CommandParameter &rhs);
        virtual ~CommandParameter();

        virtual void packBuffer(Buffer* buf) = 0;
        virtual bool unpackBuffer(Buffer* buf) = 0;
        virtual CommandParameter* clone() = 0;
        virtual bool setValueFromString(const std::string &nval) = 0;

        std::string getName();
        std::string getDescription();

        void setName(const std::string &nn);
        void setDescription(const std::string &nd);

      private:
        std::string name;
        std::string description;
    };

    /*! \brief Holds a string CommandParameter.
    */
    class StringCommandParameter : public CommandParameter{
      public:
        StringCommandParameter();
        StringCommandParameter(const StringCommandParameter &rhs);
        ~StringCommandParameter();

        void packBuffer(Buffer* buf);
        bool unpackBuffer(Buffer* buf);
        CommandParameter* clone();
        bool setValueFromString(const std::string &nval);

        unsigned int getMaxLength();
        std::string getString();

        void setString(const std::string &nval);
      
      private:
        unsigned int maxlength;
        std::string value;
    };

    /*! \brief A CommandParameter that holds an integer.
    */
    class IntegerCommandParameter : public CommandParameter{
      public:
        IntegerCommandParameter();
        IntegerCommandParameter(const IntegerCommandParameter &rhs);
        ~IntegerCommandParameter();

        void packBuffer(Buffer* buf);
        bool unpackBuffer(Buffer* buf);
        CommandParameter* clone();
        bool setValueFromString(const std::string &nval);
        
        uint32_t getValue() const;

        void setValue(uint32_t nval);

      private:
        uint32_t value;
    };

}

#endif
