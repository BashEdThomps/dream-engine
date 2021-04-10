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
  File::File
  (const string& path)
    : mPath(path),
      mStringData("")
  {
    LOG_DEBUG("File: Constructor {}" , mPath );
  }

  string
  File::getDirectory
  ()
  const
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

  string
  File::getPath
  ()
  const
  {
    return mPath;
  }

  string
  File::readString
  ()
  {
    if (mPath.empty()) return mStringData;

    if (!readBinary())
    {
      return mStringData;
    }

    mStringData = string(mBinaryData.begin(), mBinaryData.end());

    return mStringData;

  }

  bool
  File::readBinary
  ()
  {
    LOG_TRACE("File: {}", __FUNCTION__);

    if (mPath.empty()) return false;

    if (!exists())
    {
      LOG_ERROR("File: Cannot read file {} does not exist",mPath);
      return false;
    }

    std::ifstream infile;
    infile.open(mPath.c_str(), std::ios::binary);
    infile.seekg(0, std::ios::end);
    size_t binaryDataSize = infile.tellg();

    LOG_TRACE("File: binary size {}", binaryDataSize);

    mBinaryData.clear();
    mBinaryData.resize(binaryDataSize);

    infile.seekg(0, std::ios::beg);
    infile.read((char*)&mBinaryData[0], binaryDataSize);
    infile.close();

    return true;

  }

  bool
  File::writeBinary
  (const vector<uint8_t>& data)
  const
  {
    if (mPath.empty()) return false;

    auto file = fopen(mPath.c_str(),"wb");
    auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
    fclose(file);
    return bytesWritten == data.size();
  }

  bool
  File::writeString
  (const std::string& data)
  const
  {
    if (mPath.empty()) return false;

    auto file = fopen(mPath.c_str(),"wb");
    auto bytesWritten = fwrite(&data[0],sizeof(char),data.size(),file);
    fclose(file);
    return bytesWritten == data.size();
  }

  bool
  File::deleteFile
  ()
  const
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

  bool
  File::exists
  ()
  const
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

  string
  File::getNameWithExtension
  ()
  const
  {
    if (mPath.empty()) return "";

    auto endOfPath = mPath.find_last_of('/');
    auto fileName = mPath.substr(endOfPath+1);
    LOG_TRACE("Got file name with extension {}",fileName);
    return fileName;
  }

  string
  File::getNameWithoutExtension
  ()
  const
  {
    if (mPath.empty()) return "";

    auto name = getNameWithExtension();
    auto extStart = name.find_last_of(".");
    auto nameOnly = name.substr(0,extStart);
    LOG_TRACE("Got file name without extension {}",nameOnly);
    return nameOnly;
  }

  string
  File::getExtension
  ()
  const
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

  string
  File::getStringData
  ()
  const
  {
    return mStringData;
  }

  vector<uint8_t>&
  File::getBinaryData
  ()
  {
    return mBinaryData;
  }
}
