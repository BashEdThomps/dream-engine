//
// Created by Ashley Thompson on 04/01/21.
//

#include "AndroidFile.h"
#include "AndroidStorageManager.h"

#include <cstring>
#include <android/asset_manager.h>

namespace octronic::dream::android
{
   AndroidFile::AndroidFile(AndroidStorageManager* sm, const string& path)
      : File(path),
        mStorageManager(sm)
   {
       LOG_TRACE("AndroidFile: {} with asset handle {}", __FUNCTION__, path);
       if (mPath.starts_with('/')) mPath = string(&mPath.c_str()[1]);
       LOG_TRACE("AndroidFile: Android path remove leading '/' to become {}", mPath);
   }

    bool AndroidFile::readBinary()
    {
        LOG_DEBUG("AndroidFile: {}",__FUNCTION__);
        AAsset* asset = AAssetManager_open(mStorageManager->getAssetManager(), mPath.c_str(), AASSET_MODE_BUFFER);
        if (asset == nullptr)
        {
            LOG_ERROR("AndroidFile: ReadBinary asset == nullptr");
            return false;
        }
        mBinaryDataSize = AAsset_getLength(asset);

        LOG_TRACE("AndroidFile: Asset length {}", mBinaryDataSize);

        const void* data = AAsset_getBuffer(asset);

        if (data != nullptr)
        {
            LOG_TRACE("AndroidFile: Got valid buffer");
            mBinaryData = new uint8_t[mBinaryDataSize];
            memcpy(mBinaryData, data, mBinaryDataSize);
            AAsset_close(asset);
            return true;
        }
        LOG_ERROR("AndroidFile: Got nullptr buffer");
        mBinaryDataSize = 0;
        mBinaryData = nullptr;
        AAsset_close(asset);
        return false;
    }

    bool AndroidFile::exists() const
    {
        LOG_DEBUG("AndroidFile: {}",__FUNCTION__);
       return true;
    }
}