#pragma once

#include "Base/UseCountable.h"

#include <vector>
#include <string>

using std::string;
using std::vector;

namespace octronic::dream
{
  class File : public UseCountable
  {

  public:
    File(const string& path);

    File(File&) = delete;
    File& operator=(const File&) = delete;

    File(File&&) = default;
    File& operator=(File&&) = default;

    string getDirectory() const;
    string getPath() const;

    string readString();
    string getStringData() const;
    bool writeString(const string&) const;

    virtual bool readBinary();
    vector<uint8_t>& getBinaryData();
    virtual bool writeBinary(const vector<uint8_t>& data) const;

    bool deleteFile() const;
    virtual bool exists() const;

    string getNameWithExtension() const;
    string getNameWithoutExtension() const;
    string getExtension() const;

  protected:
    string mPath;
    string mStringData;
    vector<uint8_t> mBinaryData;
  };
}

