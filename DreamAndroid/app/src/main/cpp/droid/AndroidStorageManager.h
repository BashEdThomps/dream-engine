#pragma once

#include <DreamCore.h>
#include <android/asset_manager.h>

using octronic::dream::StorageManager;
using octronic::dream::File;
using std::string;

namespace octronic::dream::android
{
  class AndroidStorageManager : public StorageManager
  {
  public:
      AndroidStorageManager(AAssetManager* am);
      File* openFile(const string& path) override;
      Directory* openDirectory(const string& path) override;
      AAssetManager* getAssetManager() const;
  private:
      AAssetManager* mAssetManager;
  };
}

