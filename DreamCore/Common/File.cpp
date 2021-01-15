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

#include "Common/Logger.h"
#include "Common/Constants.h"

#include <fstream>
#include <sstream>
#include <cstdio>

using std::stringstream;
using std::ifstream;

namespace Dream
{
    File::File
    (string path)
        : mPath(path)
    {
    }

    File::~File
    ()
    {
        LOG_DEBUG( "File: Destroying reader for {}" , mPath );
    }

    string File::getDirectory() const
    {
        auto endOfPath = mPath.find_last_of(Constants::DIR_PATH_SEP);
        if (endOfPath == string::npos)
        {
            endOfPath = mPath.find_last_of(Constants::DIR_PATH_SEP_WINDOWS);
        }
        if (endOfPath == string::npos)
        {
            return "";
        }
        return mPath.substr(0,endOfPath);
    }

    string
    File::getPath
    () const
    {
        return mPath;
    }

    string
    File::readString
    () const
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
    () const
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
    (const vector<char>& data) const
    {
        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
        fclose(file);
        return bytesWritten == data.size();
    }

    bool
    File::writeString
    (const std::string& data) const
    {
        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
        fclose(file);
        return bytesWritten == data.size();
    }

    bool
    File::deleteFile
    () const
    {
        LOG_DEBUG("File: Deleting file {}",mPath);
        if(remove(mPath.c_str()) != 0)
        {
            LOG_ERROR("File: Error deleting file {}",mPath );
            perror("File check error");
            return false;
        }
        return true;
    }

    bool
    File::exists
    ()
    const
    {
        FILE* file = fopen(mPath.c_str(),"rb");
        if (file)
        {
            fclose(file);
            return true;
        }
        perror("File check error");
        return false;
    }

    string
    File::nameWithExtension
    () const
    {

        auto endOfPath = mPath.find_last_of(Constants::DIR_PATH_SEP);
        auto fileName = mPath.substr(endOfPath+1);
        LOG_TRACE("File: Got file name with extension {}",fileName);
        return fileName;
    }

    string File::nameWithoutExtension() const
    {
        auto name = nameWithExtension();
        auto extStart = name.find_last_of(".");
        auto nameOnly = name.substr(0,extStart);
        LOG_TRACE("File: Got file name without extension {}",nameOnly);
        return nameOnly;
    }

    string
    File::extension
    () const
    {
        auto name = nameWithExtension();
        auto extStart = name.find_last_of(".");
        if (extStart != string::npos)
        {
            auto ext = name.substr(extStart+1);
            LOG_TRACE("File: Got file extension {}",ext);
            return ext;
        }
        return "";
    }
} // End of Dream
