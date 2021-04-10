#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Common/Uuid.h"
#include "Common/AssetType.h"

#include "Storage/StorageManager.h"
#include "Storage/File.h"
#include "Storage/Directory.h"

#include <json.hpp>

using std::string;
using std::vector;
using nlohmann::json;

namespace octronic::dream
{
  class ProjectDefinition;
  class AssetDefinition;

  /**
     * @brief The ProjectDirectory class is an interface to a Project's physical
     * location on disk. All IO should be done through the ProjectDirectory, to
     * facilitate cross-platform abstractions.
     */
  class ProjectDirectory
  {

  public:
    ProjectDirectory
    (StorageManager& sm, const string& baseDir);

    ProjectDirectory(ProjectDirectory&&) = default;
    ProjectDirectory& operator=(ProjectDirectory&&) = default;


    bool
    baseDirectoryExists
    () const;

    bool
    createBaseDirectory
    () const;

    bool
    createAllAssetDirectories
    () const;

    File&
    openAssetFile
    (AssetDefinition& def,
     const string &format = "") const;

    bool
    writeAssetData
    (AssetDefinition& def,
     const vector<uint8_t>& data,
     const string &format = "") const;

    bool
    writeAssetStringData
    (AssetDefinition& def,
     const string& data,
     const string& format = "") const;

    string
    getAssetAbsolutePath
    (AssetDefinition& def) const;

    string
    getAssetAbsolutePath
    (AssetDefinition& def,
     const string& format) const;

    string
    getAssetDirectoryPath
    (AssetDefinition& def) const;

    string
    getAssetTypeDirectory
    (AssetType type) const;

    bool
    removeAssetDirectory
    (AssetDefinition& def) const;

    string
    getProjectFilePath
    (ProjectDefinition& def) const;

    bool
    assetTypeDirectoryExists
    (AssetType type) const;

    bool
    createAssetTypeDirectory
    (AssetType type) const;

    vector<string>
    removeUnusedAssetDirectories
    (ProjectDefinition& def) const;

    bool
    saveProject
    (ProjectDefinition& pDef) const;

    string
    findProjectFileInDirectory
    (bool removeExtension) const;

    json
    readProjectDefinition() const;

    json
    createProjectDefinition() const;

    string getBasePath() const;

  protected:
    bool
    directoryContainsProject
    () const;

    bool
    checkAssetDirectoriesExist
    () const;

    bool
    selfCreateProjectDirectory
    ();

  private:
    string mBasePath;
    reference_wrapper<StorageManager> mStorageManager;
  };
}
