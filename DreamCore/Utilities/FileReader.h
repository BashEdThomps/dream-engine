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

#include "../Common/Constants.h"
#include "../Common/DreamObject.h"

using std::string;
using std::vector;

namespace Dream
{
    class FileReader : public DreamObject
    {
    private:
        string mString;
        vector<char> mBinaryVector;
        string mPath;
    public:
        FileReader(string);
        ~FileReader();

        string getPath();
        bool readIntoString();
        string getContentsAsString();
        bool readIntoBinaryVector();
        vector<char> getContentsAsBinaryVector();
        int getFileSize();

    }; // End of FileReader

} // End of Dream

