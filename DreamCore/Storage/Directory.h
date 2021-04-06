#pragma once

#include "Base/UseCountable.h"

#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::reference_wrapper;

namespace octronic::dream
{
  class File;
  class StorageManager;

  class Directory : public UseCountable
  {
  public:
    Directory(StorageManager& fileManager, const string& dir);

    Directory(const Directory&) = delete;
    Directory& operator=(const Directory&) = delete;

    Directory(Directory&&) = default;
    Directory& operator=(Directory&&) = default;

    virtual vector<string> list(const string& regex = "");
    virtual vector<string> listSubdirectories(const string& regex = "");

    string getPath() const;
    void setPath(const string& path);
    string getName() const;

    virtual bool exists() const;
    virtual bool create();
    virtual bool deleteDirectory();
    virtual File& file(const string& fileName);
    virtual bool isDirectory() const;
    StorageManager& getStorageManager() const;

  protected:
    reference_wrapper<StorageManager> mStorageManager;
    string mPath;
  };
}
