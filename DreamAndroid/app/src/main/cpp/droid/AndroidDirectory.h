#pragma once

#include <DreamCore.h>

using octronic::dream::Directory;

namespace octronic::dream::android
{
    class AndroidStorageManager;

    class AndroidDirectory : public Directory
    {
    public:
        AndroidDirectory(AndroidStorageManager* sm, const string& path);
        ~AndroidDirectory();

        vector<string> list(const string& regexStr = "") override;
        vector<string> listSubdirectories(const string& regexStr = "") override;

        bool exists() override;
        bool create() override;
        bool deleteDirectory() override;
        File* file(const string& fileName) override;
        bool isDirectory() override;
    };
}