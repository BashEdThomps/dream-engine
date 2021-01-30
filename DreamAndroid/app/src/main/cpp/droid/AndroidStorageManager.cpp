#include "AndroidStorageManager.h"

#include "AndroidFile.h"
#include "AndroidDirectory.h"

namespace octronic::dream::android
{
    AndroidStorageManager::AndroidStorageManager(AAssetManager* am)
        : StorageManager(),
        mAssetManager(am)
    {
        LOG_DEBUG("AndroidStorageManager: {}",__FUNCTION__);
    }

    File* AndroidStorageManager::openFile(const string& path)
    {
        LOG_DEBUG("AndroidStorageManager: {}",__FUNCTION__);

        auto file_itr = std::find_if(
                mOpenFiles.begin(),
                mOpenFiles.end(),
                [&](File* next_file)
                {
                    return next_file->getPath().compare(path) == 0;
                }
        );

        if (file_itr != mOpenFiles.end())
        {
            return (*file_itr);
        }
        else
        {
            AndroidFile* af = new AndroidFile(this,path);
            mOpenFiles.push_back(af);
            return af;
        }
    }

    Directory* AndroidStorageManager::openDirectory(const string& path)
    {
        LOG_DEBUG("AndroidStorageManager: {}",__FUNCTION__);
        auto dir_itr = std::find_if(
                mOpenDirectories.begin(),
                mOpenDirectories.end(),
                [&](Directory* next_dir)
                {
                    return next_dir->getPath().compare(path) == 0;
                }
        );

        if (dir_itr != mOpenDirectories.end())
        {
            return (*dir_itr);
        }
        else
        {
            AndroidDirectory* ad = new AndroidDirectory(this, path);
            mOpenDirectories.push_back(ad);
            return ad;
        }
    }

    AAssetManager* AndroidStorageManager::getAssetManager() const
    {
        LOG_DEBUG("AndroidStorageManager: {}",__FUNCTION__);
        return mAssetManager;
    }
}
