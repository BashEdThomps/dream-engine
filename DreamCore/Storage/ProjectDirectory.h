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
#include "Common/Constants.h" // AssetType

using std::shared_ptr;
using std::string;
using std::vector;
using std::enable_shared_from_this;
using std::weak_ptr;

namespace octronic::dream
{
    class Project;
    class File;
    class AssetDefinition;
    class StorageManager;
    class Directory;

    /**
     * @brief The ProjectDirectory class is an interface to a Project's physical
     * location on disk. All IO should be done through the ProjectDirectory, to
     * facilitate cross-platform abstractions.
     */
    class ProjectDirectory : public enable_shared_from_this<ProjectDirectory>
    {
    public:
        ProjectDirectory(const weak_ptr<StorageManager>& fm);
        ProjectDirectory(const weak_ptr<StorageManager>& fm, const weak_ptr<Project>& project);
        ~ProjectDirectory();

        bool baseDirectoryExists() const;
        bool createBaseDirectory() const;
        bool createAllAssetDirectories() const;

        weak_ptr<File> readAssetData(const weak_ptr<AssetDefinition>&, const string &format = "") const;
        bool writeAssetData(const weak_ptr<AssetDefinition>&, const vector<uint8_t>& data, const string &format = "") const;
        bool writeAssetStringData(const weak_ptr<AssetDefinition>&, const string& data, const string &format = "") const;

        string getAssetAbsolutePath(UuidType) const;
        string getAssetAbsolutePath(const weak_ptr<AssetDefinition>&) const;
        string getAssetAbsolutePath(const weak_ptr<AssetDefinition>&, const string &format) const;
        string getAssetDirectoryPath(const weak_ptr<AssetDefinition>&) const;
        string getAssetTypeDirectory(AssetType type, const string& base = "") const;

        bool removeAssetDirectory(const weak_ptr<AssetDefinition>&) const;

        string getProjectFilePath() const;
        bool assetTypeDirectoryExists(AssetType type) const;

        bool createAssetTypeDirectory(AssetType type) const;
        vector<string> cleanupAssetsDirectory() const;

        shared_ptr<Project> newProject(const weak_ptr<Directory>& projectDir);
        shared_ptr<Project> openFromFile(const weak_ptr<File>& file);
        shared_ptr<Project> openFromDirectory(const weak_ptr<Directory>& directory);
        bool saveProject() const;
        void closeProject();

    protected:
        string findProjectFileInDirectory(const weak_ptr<Directory>& dir) const;
        bool directoryContainsProject(const weak_ptr<Directory>& dir) const;
        bool findAssetDirectories(const weak_ptr<Directory>& dir) const;

    private:
        weak_ptr<StorageManager> mStorageManager;
        shared_ptr<Project> mProject;
        string mPath;
    };
}
