/*
* Dream::Util::FileReader
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

namespace Dream {
  FileReader::FileReader(std::string path) {
    mPath = path;
    mStringStream = NULL;
    mBinaryVector = NULL;
  }

  FileReader::~FileReader(void) {
    if (mInputStream.is_open()) {
      mInputStream.close();
    }

    if (mBinaryVector != NULL) {
      delete mBinaryVector;
    }

    if (mStringStream != NULL) {
      delete mStringStream;
    }
  }

  std::string FileReader::getPath() {
    return mPath;
  }

  bool FileReader::readIntoStringStream() {
    mInputStream.open(mPath.c_str(), std::ifstream::in);
    if (mInputStream.is_open()) {
      mStringStream = new std::stringstream();
      std::string line;
      while ( getline (mInputStream,line) ) {
          *mStringStream << line << '\n';
        }
        mInputStream.close();
      return true;
    } else {
      return false;
    }
  }

  std::string FileReader::getContentsAsString() {
    if (mStringStream != NULL) {
      return mStringStream->str();
    } else {
      return "";
    }
  }

  bool FileReader::readIntoBinaryVector() {
    mInputStream.open(mPath.c_str(), std::ios::binary );
    mBinaryVector = new std::vector<char>((std::istreambuf_iterator<char>(mInputStream)), (std::istreambuf_iterator<char>()));
    mInputStream.close();
    return mBinaryVector->size() > 0;
  }

  std::vector<char>* FileReader::getContentsAsBinaryVector() {
    return mBinaryVector;
  }

  int FileReader::getFileSize() {
    if (mStringStream != NULL) {
      return getContentsAsString().size();
    }
    else if (mBinaryVector != NULL) {
        return getContentsAsBinaryVector()->size();
      }
    else {
      return -1;
    }
  }
} // End of Dream
