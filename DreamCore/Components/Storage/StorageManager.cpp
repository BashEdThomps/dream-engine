#include "StorageManager.h"

#include "File.h"
#include "Directory.h"
#include "Common/Logger.h"



namespace octronic::dream
{
    StorageManager::StorageManager()
        :LockableObject("StorageManager")
    {

        LOG_TRACE("StorageManager: {}", __FUNCTION__);
    }

    StorageManager::~StorageManager()
    {
    }

    File* StorageManager::openFile(const string& file_path)
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("StorageManager: {} {}", __FUNCTION__, file_path);

            auto file_itr = std::find_if( mOpenFiles.begin(), mOpenFiles.end(),
                                          [&](File* next_file)
            {
                    return next_file->getPath().compare(file_path) == 0;
        }
                    );

            if (file_itr != mOpenFiles.end())
            {
                return (*file_itr);
            }
            else
            {
                File* f = new File(file_path);
                mOpenFiles.push_back(f);
                return f;
            }
        } dreamElseLockFailed
    }

    void StorageManager::closeFile(File *file)
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("StorageManager: {} {}", __FUNCTION__, file->getPath());
            auto file_itr = std::find_if(mOpenFiles.begin(), mOpenFiles.end(),
                                         [&](File* next_file)
            {
                    return next_file->getPath().compare(file->getPath()) == 0;
        }
                    );

            if (file_itr != mOpenFiles.end())
            {
                delete (*file_itr);
                mOpenFiles.erase(file_itr);
            }
            else
            {
                LOG_ERROR("StorageManager: FATAL - Attempted to close File not opened by StorageManager");
                assert(false);
            }
        } dreamElseLockFailed
    }

    Directory* StorageManager::openDirectory(const string& path)
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("StorageManager: {} {}", __FUNCTION__, path);

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
                Directory* d = new Directory(this, path);
                mOpenDirectories.push_back(d);
                return d;
            }
        } dreamElseLockFailed
    }

    void StorageManager::closeDirectory(Directory* d)
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("StorageManager: {} {}", __FUNCTION__, d->getPath());
            auto dir_itr = std::find_if(
                        mOpenDirectories.begin(),
                        mOpenDirectories.end(),
                        [&](Directory* next_dir)
            {
                    return next_dir->getPath().compare(d->getPath()) == 0;
        }
                    );

            if (dir_itr != mOpenDirectories.end())
            {
                delete (*dir_itr);
                mOpenDirectories.erase(dir_itr);
            }
            else
            {
                LOG_ERROR("StorageManager: FATAL - Attempted to close Directory not opened by StorageManager");
                assert(false);
            }
        } dreamElseLockFailed
    }
}
