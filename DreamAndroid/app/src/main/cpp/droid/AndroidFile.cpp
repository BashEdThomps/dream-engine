//
// Created by Ashley Thompson on 04/01/21.
//

#include "AndroidFile.h"

#include "AndroidFileManager.h"

#include <cstring>
#include <android/asset_manager.h>

namespace octronic::dream::android
{
   AndroidFile::AndroidFile(AndroidFileManager* fm, string path)
      : File(path),
        mFileManager(fm)
   {
      LOG_TRACE("AndroidFile: {} with asset handle {}", __FUNCTION__, path);
   }

    bool AndroidFile::ReadBinary()
    {
        AAsset* asset = AAssetManager_open(mFileManager->GetAssetManager(), mPath.c_str(), AASSET_MODE_BUFFER);
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

    bool AndroidFile::Exists() const
    {
       return true;
    }
}