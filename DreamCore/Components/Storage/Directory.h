#pragma once

#include "Base/LockableObject.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace octronic::dream
{
    class File;
    class StorageManager;

    class Directory : public LockableObject
    {
        friend class StorageManager;
    public:
        Directory(StorageManager* fileManager, const string& dir);
        ~Directory();
        virtual vector<string> list(const string& regex = "");
        virtual vector<string> listSubdirectories(const string& regex = "");

        string getPath() const;
        void setPath(const string& path);
        string getName();

        virtual bool exists();
        virtual bool create();
        virtual bool deleteDirectory();
        virtual File* file(const string& fileName);
        virtual bool isDirectory();

    protected:

        StorageManager* mStorageManager;
        string mPath;
    };

}
