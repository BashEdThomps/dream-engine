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

        string getDirectory() const;
        string getPath() const;
        string readString() const;
        vector<char> readBinary() const;
        int getFileSize() const;
        bool writeBinary(const vector<char>&) const;
        bool writeString(const string&) const;
        bool deleteFile() const;
        bool exists() const;
        string nameWithExtension() const;
        string nameWithoutExtension() const;
        string extension() const;
    };
}

