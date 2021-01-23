/*
* File
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

#include <fstream>
#include <sstream>
#include <cstdio>

using std::stringstream;
using std::ifstream;
using std::ios;
using std::istreambuf_iterator;

namespace octronic::dream
{
    File::File(string path)
        : mPath(path),
          mBinaryData(nullptr),
          mBinaryDataSize(0),
          mStringData("")
	{
        LOG_DEBUG( "File:  {} {}" ,__FUNCTION__, mPath );
    }

    File::~File()
    {
        LOG_DEBUG( "File: {} {}" ,__FUNCTION__, mPath );
        if (mBinaryData != nullptr && mBinaryDataSize != 0)
        {
            delete[] mBinaryData;
            mBinaryData = nullptr;
            mBinaryDataSize = 0;
        }
    }

    string File::getDirectory() const
    {
        if (mPath.empty()) return "";

        auto endOfPath = mPath.find_last_of('/');
        if (endOfPath == string::npos)
        {
            endOfPath = mPath.find_last_of('\\');
        }
        if (endOfPath == string::npos)
        {
            return "";
        }
        return mPath.substr(0,endOfPath);
    }

    string File::getPath() const
    {
        return mPath;
    }

    string File::readString()
    {
        if (mPath.empty()) return mStringData;

        if (!readBinary())
        {
            return mStringData;
        }

        // Null termination not guaranteed, manually terminating the string
        // seems like a good idea

        char* temp_str = new char[mBinaryDataSize + 1];

        strncpy(temp_str, (const char*)mBinaryData, mBinaryDataSize);
        temp_str[mBinaryDataSize] = 0;

        mStringData = std::move(string(temp_str));
        delete[] temp_str;

        delete[] mBinaryData;
        mBinaryData = nullptr;
        mBinaryDataSize = 0;

        return mStringData;
    }

    bool File::readBinary()
    {
        LOG_TRACE("File: {}", __FUNCTION__);

        if (mPath.empty()) return false;

        std::ifstream infile;
        infile.open(mPath.c_str(), std::ios::binary);
        infile.seekg(0, std::ios::end);
        mBinaryDataSize = infile.tellg();

        LOG_TRACE("File: binary size {}", mBinaryDataSize);

        mBinaryData = new uint8_t[mBinaryDataSize];
        memset(mBinaryData,0,sizeof(uint8_t) * mBinaryDataSize);

        infile.seekg(0, std::ios::beg);
        infile.read((char*)mBinaryData, mBinaryDataSize);
        infile.close();

        return true;
    }

    bool File::writeBinary (const uint8_t* data, size_t size) const
    {
        if (mPath.empty()) return false;

        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(data,sizeof(char),size,file);
        fclose(file);
        return bytesWritten == size;
    }

    bool File::writeString(const std::string& data) const
    {
        if (mPath.empty()) return false;

        auto file = fopen(mPath.c_str(),"wb");
        auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
        fclose(file);
        return bytesWritten == data.size();
    }

    bool File::deleteFile() const
    {
        if (mPath.empty()) return false;

        LOG_DEBUG("File: Deleting file {}",mPath);
        if(remove(mPath.c_str()) != 0)
        {
            LOG_ERROR("File: Error deleting file {}",mPath );
            return false;
        }
        return true;
    }

    bool File::exists() const
    {
        if (mPath.empty()) return false;

        FILE* file = fopen(mPath.c_str(),"rb");
        if (file)
        {
            fclose(file);
            return true;
        }
        LOG_ERROR("File: Exist check error");
        return false;
    }

    string File::getNameWithExtension() const
    {
        if (mPath.empty()) return "";

        auto endOfPath = mPath.find_last_of('/');
        auto fileName = mPath.substr(endOfPath+1);
        LOG_TRACE("Got file name with extension {}",fileName);
        return fileName;
    }

    string File::getNameWithoutExtension() const
    {
        if (mPath.empty()) return "";

        auto name = getNameWithExtension();
        auto extStart = name.find_last_of(".");
        auto nameOnly = name.substr(0,extStart);
        LOG_TRACE("Got file name without extension {}",nameOnly);
        return nameOnly;
    }

    string File::getExtension() const
    {
        if (mPath.empty()) return "";

        auto name = getNameWithExtension();
        auto extStart = name.find_last_of(".");
        if (extStart != string::npos)
        {
            auto ext = name.substr(extStart+1);
            LOG_TRACE("Got file extension {}",ext);
            return ext;
        }
        return "";
    }

    size_t File::getBinaryDataSize() const
    {
        return mBinaryDataSize;
    }

    string File::getStringData() const
    {
        return mStringData;
    }

    uint8_t* File::getBinaryData() const
    {
        return mBinaryData;
    }
}
