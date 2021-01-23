//
// Created by Ashley Thompson on 04/01/21.
//

#include "AndroidFileManager.h"
#include "AndroidFile.h"

namespace octronic::dream::android
{
    AndroidFileManager::AndroidFileManager(AAssetManager* am)
        : mAssetManager(am)
    {
        LOG_TRACE("AndroidFileManager: {}", __FUNCTION__);
    }

    File* AndroidFileManager::OpenFile(string path)
    {
        LOG_TRACE("AndroidFileManager: {}", __FUNCTION__);
        AndroidFile* file = new AndroidFile(this, path);
        return file;
    }

    AAssetManager* AndroidFileManager::GetAssetManager() const
    {
        return mAssetManager;
    }
}
