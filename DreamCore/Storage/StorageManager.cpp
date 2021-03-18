#include "StorageManager.h"

#include "File.h"
#include "Directory.h"
#include "Common/Logger.h"

#include <memory>

using std::make_shared;

namespace octronic::dream
{
    StorageManager::StorageManager
    ()
    {
        LOG_TRACE("StorageManager: {}", __FUNCTION__);
    }

    StorageManager::~StorageManager
    ()
    {
    }

    weak_ptr<File>
    StorageManager::openFile
    (const string& file_path)
    {
        LOG_TRACE("StorageManager: {} {}", __FUNCTION__, file_path);

        auto file_itr = std::find_if(
                    mOpenFiles.begin(),
                    mOpenFiles.end(),
                    [&](shared_ptr<File>& next_file)
        			{return next_file->getPath().compare(file_path) == 0;});

        if (file_itr != mOpenFiles.end())
        {
            return (*file_itr);
        }
        else
        {
            shared_ptr<File> f = make_shared<File>(file_path);
            mOpenFiles.push_back(f);
            return f;
        }
    }

    void
    StorageManager::closeFile
    (const weak_ptr<File>& file)
    {
        auto fileLock = file.lock();
        LOG_TRACE("StorageManager: {} {}", __FUNCTION__, fileLock->getPath());
        auto file_itr = std::find_if(mOpenFiles.begin(), mOpenFiles.end(),
        	[&](const shared_ptr<File>& next_file)
        	{ return next_file->getPath().compare(fileLock->getPath()) == 0; });

        if (file_itr != mOpenFiles.end())
        {
            mOpenFiles.erase(file_itr);
        }
        else
        {
            LOG_ERROR("StorageManager: FATAL - Attempted to close File not opened by StorageManager");
            assert(false);
        }
    }

    weak_ptr<Directory>
    StorageManager::openDirectory
    (const string& path)
    {
        LOG_TRACE("StorageManager: {} {}", __FUNCTION__, path);

        auto dir_itr = std::find_if(
                    mOpenDirectories.begin(),
                    mOpenDirectories.end(),
                    [&](const shared_ptr<Directory>& next_dir)
        { return next_dir->getPath().compare(path) == 0; });

        if (dir_itr != mOpenDirectories.end())
        {
            return (*dir_itr);
        }
        else
        {
            shared_ptr<Directory> d = make_shared<Directory>(shared_from_this(), path);
            mOpenDirectories.push_back(d);
            return d;
        }
    }

    void
    StorageManager::closeDirectory
    (const weak_ptr<Directory>& d)
    {
        auto dirLock = d.lock();
        LOG_TRACE("StorageManager: {} {}", __FUNCTION__, dirLock->getPath());
        auto dir_itr = std::find_if( mOpenDirectories.begin(), mOpenDirectories.end(),
            [&](const shared_ptr<Directory>& next_dir)
        	{ return next_dir->getPath().compare(dirLock->getPath()) == 0; });

        if (dir_itr != mOpenDirectories.end())
        {
            mOpenDirectories.erase(dir_itr);
        }
        else
        {
            LOG_ERROR("StorageManager: FATAL - Attempted to close Directory not opened by StorageManager");
            assert(false);
        }
    }
}
