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

#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Constants.h"
#include <memory>

using namespace std;

namespace Dream
{


    class FileReader
    {
    private:
        unique_ptr<stringstream> mStringStream;
        unique_ptr<vector<char>> mBinaryVector;
        ifstream     mInputStream;
        string       mPath;
    public:
        FileReader(string);
        ~FileReader(void);

        string getPath();
        bool readIntoStringStream();
        string getContentsAsString();
        bool readIntoBinaryVector();
        vector<char>* getContentsAsBinaryVector();
        int getFileSize();

    }; // End of FileReader
} // End of Dream

#endif // End of JSONFILEREADER_H
