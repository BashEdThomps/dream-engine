/*
* FileReader
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

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../Common/DreamObject.h"

using std::string;
using std::vector;

namespace Dream
{
    class File : public DreamObject
    {
    private:
        string mPath;
    public:
        File(string);
        ~File();

        string getPath();
        string readString();
        vector<char> readBinary();
        int getFileSize();
        bool writeBinary(vector<char>);
        bool writeString(string);

    }; // End of FileReader

} // End of Dream

