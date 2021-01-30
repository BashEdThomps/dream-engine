#include "AndroidDirectory.h"

#include "AndroidStorageManager.h"

#include <regex>

using std::cmatch;
using std::regex;

namespace octronic::dream::android
{
    AndroidDirectory::AndroidDirectory(AndroidStorageManager* sm, const string& path)
        : Directory(sm, path)
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
    }

    AndroidDirectory::~AndroidDirectory()
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
    }

    vector<string> AndroidDirectory::list(const string& regexStr)
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
        vector<string> contents;
        regex fileRegex(regexStr);
        cmatch match;
        bool usingRegex = !regexStr.empty();

        AndroidStorageManager* sm = static_cast<AndroidStorageManager*>(mStorageManager);
        AAssetDir* dir = AAssetManager_openDir(sm->getAssetManager(), mPath.c_str());

        if (dir)
        {
           AAssetDir_rewind(dir);
           while (true)
           {
               const char* nextEntry = AAssetDir_getNextFileName(dir);
               if (nextEntry == nullptr) break;

               string fileName(nextEntry);
               LOG_DEBUG("AndroidDirectory: Next entry {}", nextEntry);

               if (fileName[0] == '.') continue;

               if (usingRegex)
               {
                   if (regex_search(fileName.c_str(),match,fileRegex))
                   {
                       contents.push_back(fileName);
                   }
               }
               else
               {
                   contents.push_back(fileName);
               }
           }
           AAssetDir_close(dir);
           dir = nullptr;
        }
        return contents;
    }

    vector<string> AndroidDirectory::listSubdirectories(const string& regex)
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
        vector<string>  retval;
        return retval;
    }

    bool AndroidDirectory::exists()
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
        return false;
    }

    bool AndroidDirectory::create()
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
        return false;
    }

    bool AndroidDirectory::deleteDirectory()
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
        return false;
    }

    File* AndroidDirectory::file(const string& fileName)
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
       return nullptr;
    }

    bool AndroidDirectory::isDirectory()
    {
        LOG_DEBUG("AndroidDirectory: {}",__FUNCTION__);
        return false;
    }
}
