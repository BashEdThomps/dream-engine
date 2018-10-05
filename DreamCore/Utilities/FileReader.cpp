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

#include "FileReader.h"

#include <fstream>
#include <sstream>

using std::stringstream;
using std::ifstream;

namespace Dream
{

    FileReader::FileReader
    (string path)
        : DreamObject ("FileReader")
    {
        mPath = path;
    }

    FileReader::~FileReader
    ()
    {
        auto log = getLog();
         log->debug( "FileReader: Destroying reader for {}" , mPath );
    }

    string
    FileReader::getPath
    ()
    {
        return mPath;
    }

    bool
    FileReader::readIntoString
    ()
    {
        ifstream mInputStream;
        stringstream mStringStream;

        mInputStream.open(mPath.c_str(), ifstream::in);
        if (mInputStream.is_open())
        {
            string line;
            while ( getline (mInputStream,line) )
            {
                mStringStream << line << '\n';
            }
            mString = mStringStream.str();
            mInputStream.close();
            return true;
        }
        else
        {
            return false;
        }
    }

    string
    FileReader::getContentsAsString
    ()
    {
        return mString;
    }

    bool
    FileReader::readIntoBinaryVector
    ()
    {
        ifstream mInputStream;

        mInputStream.open(mPath.c_str(), ios::binary );
        mBinaryVector = vector<char>
        (
            (istreambuf_iterator<char>(mInputStream)),
            (istreambuf_iterator<char>())
        );
        mInputStream.close();
        return mBinaryVector.size() > 0;
    }

    vector<char>
    FileReader::getContentsAsBinaryVector
    ()
    {
        return mBinaryVector;
    }

    int
    FileReader::getFileSize
    ()
    {
        if (mString.size() > 0)
        {
            return static_cast<int>(mString.size());
        }
        else if (!mBinaryVector.empty())
        {
            return static_cast<int>(mBinaryVector.size());
        }
        else
        {
            return -1;
        }
    }

} // End of Dream
