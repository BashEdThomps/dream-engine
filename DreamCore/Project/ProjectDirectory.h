/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
