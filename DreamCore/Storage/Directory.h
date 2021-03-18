#pragma once

#include <string>
#include <vector>
#include <memory>

using std::shared_ptr;
using std::string;
using std::vector;
using std::weak_ptr;
using std::enable_shared_from_this;

namespace octronic::dream
{
    class File;
    class StorageManager;

    class Directory : public enable_shared_from_this<Directory>
    {
        friend class StorageManager;
    public:
        Directory(const weak_ptr<StorageManager>& fileManager, const string& dir);
        ~Directory();
        virtual vector<string> list(const string& regex = "");
        virtual vector<string> listSubdirectories(const string& regex = "");

        string getPath() const;
        void setPath(const string& path);
        string getName() const;

        virtual bool exists() const;
        virtual bool create();
        virtual bool deleteDirectory();
        virtual weak_ptr<File> file(const string& fileName);
        virtual bool isDirectory() const;

    protected:
        weak_ptr<StorageManager> mStorageManager;
        string mPath;
    };

}
