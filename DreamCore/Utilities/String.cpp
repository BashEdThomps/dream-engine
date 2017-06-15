/*
* StringUtils
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "String.h"

namespace Dream
{

    String::String(void) {}
    String::~String(void) {}

    string String::indent(int level)
    {
        string retval = "";
        for (int i=0;i<level;i++)
        {
            retval += "  ";
        }
        retval += "* ";
        return retval;
    }

    string String::boolToYesNo(bool value)
    {
        return (value ? "Yes" : "No");
    }

    string String::floatVectorToString(vector<float> vector)
    {
        stringstream outputStream;
        outputStream << "(";
        size_t vectorSize = vector.size();
        for (size_t i=0; i<vectorSize; i++)
        {
            outputStream << vector[i];
            if (i<vectorSize-1)
            {
                outputStream << ",";
            }
        }
        outputStream << ")";
        return outputStream.str();
    }

    string String::vec3ToString(glm::vec3 vector)
    {
        stringstream outputStream;
        outputStream << "(";
        outputStream << vector.x;
        outputStream << ",";
        outputStream << vector.y;
        outputStream << ",";
        outputStream << vector.z;
        outputStream << ")";
        return outputStream.str();
    }
}   // End of Dream
