/*
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

#include <string>

using std::string;

namespace octronic::dream
{
    class File
    {
		friend class StorageManager;

    public:
        string getDirectory() const;
        string getPath() const;

        string readString();
		string getStringData() const;
        bool writeString(const string&) const;

        virtual bool readBinary();
        size_t getBinaryDataSize() const;
		uint8_t* getBinaryData() const;
        virtual bool writeBinary(const uint8_t* data, size_t size) const;

        bool deleteFile() const;
        virtual bool exists() const;

        string getNameWithExtension() const;
        string getNameWithoutExtension() const;
        string getExtension() const;

    private:
		File(string path);
        ~File();
        File(const File&) = delete;
        File& operator=(const File&) = delete;

	protected:
        string mPath;
		string mStringData;
		uint8_t*  mBinaryData;
		size_t mBinaryDataSize;
    };
}

