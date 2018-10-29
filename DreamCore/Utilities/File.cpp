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

#include "File.h"

#include <fstream>
#include <sstream>

using std::stringstream;
using std::ifstream;

namespace Dream
{
    File::File
    (string path)
        : DreamObject ("FileReader")
    {
        mPath = path;
    }

    File::~File
    ()
    {
        auto log = getLog();
         log->debug( "FileReader: Destroying reader for {}" , mPath );
    }

    string
    File::getPath
    ()
    {
        return mPath;
    }

    string
    File::readString
    ()
    {
        ifstream inputStream;
        stringstream stringStream;

        inputStream.open(mPath.c_str(), ifstream::in);
        if (inputStream.is_open())
        {
            string line;
            while ( getline (inputStream,line) )
            {
                stringStream << line << '\n';
            }
            auto data = stringStream.str();
            inputStream.close();
            return data;
        }
        else
        {
            return "";
        }
    }

    vector<char>
    File::readBinary
    ()
    {
        vector<char> data;
        ifstream inputStream;
        inputStream.open(mPath.c_str(), ios::binary );
        data = vector<char>
        (
            (istreambuf_iterator<char>(inputStream)),
            (istreambuf_iterator<char>())
        );
        inputStream.close();
        return data;
    }

    bool
    File::writeBinary
    (vector<char> data)
    {
        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
        fclose(file);
        return bytesWritten = data.size();
    }

    bool
    File::writeString
    (string data)
    {
        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
        fclose(file);
        return bytesWritten = data.size();
    }


} // End of Dream
