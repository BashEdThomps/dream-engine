//
// Created by Ashley Thompson on 04/01/21.
//
#pragma once

#include <CanDashCore.h>
#include <android/asset_manager.h>

using octronic::dream::FileManager;

namespace octronic::dream::android
{
  class File;
  class AndroidFileManager : public FileManager
  {
  public:
      AndroidFileManager(AAssetManager* am);
      File* openFile(string path) override;
      AAssetManager* getAssetManager() const;
  private:
      AAssetManager* mAssetManager;
  };
}

