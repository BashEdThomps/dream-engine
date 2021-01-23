#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace octronic::dream
{
	class File;
	class StorageManager;
    class Directory
    {
		friend class StorageManager;
    public:
        vector<string> list(const string& regex = "");
        vector<string> listSubdirectories(const string& regex = "");

        string getPath() const;
        void setPath(const string& path);
        string getName();

        bool exists();
        bool create();
        bool deleteDirectory();
        File* file(const string& fileName);
        bool isDirectory();

    private:
		Directory(StorageManager* fileManager, const string& dir);
        ~Directory();
		StorageManager* mStorageManager;
        string mPath;
    };

}
