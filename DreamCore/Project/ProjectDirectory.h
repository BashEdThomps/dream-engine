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

#include "Common/ArgumentParser.h"
#include "Common/Constants.h"
#include "Common/File.h"

namespace Dream
{
    class Project;
    class AssetDefinition;

    class ProjectDirectory
    {
    public:
        ProjectDirectory();
        ProjectDirectory(Project* project);
        ~ProjectDirectory();

        bool baseDirectoryExists() const;
        bool createBaseDirectory() const;
        bool createAllAssetDirectories() const;

        vector<char> readAssetData(AssetDefinition*, const string &format = "") const;
        bool writeAssetData(AssetDefinition*, const vector<char> &, const string &format = "") const;

        string getAssetAbsolutePath(uint32_t) const;
        string getAssetAbsolutePath(AssetDefinition*) const;
        string getAssetAbsolutePath(AssetDefinition*, const string &format) const;
        string getAssetDirectoryPath(AssetDefinition*) const;
        string getAssetTypeDirectory(AssetType type, const string &base = "") const;

        bool removeAssetDirectory(AssetDefinition*) const;

        string getProjectFilePath() const;
        bool assetTypeDirectoryExists(AssetType type) const;

        bool createAssetTypeDirectory(AssetType type) const;
        vector<uint32_t> cleanupAssetsDirectory() const;

        Project* newProject(const string& projectDir);
        Project* openFromFileReader(const string& directory, const File& fileReader);
        Project* openFromArgumentParser(const ArgumentParser& parser);
        Project* openFromDirectory(const string& directory);
        bool saveProject() const;
        void closeProject();

    protected:
        string findProjectFileInDirectory(const string& dir) const;
        bool directoryContainsProject(const string& dir) const;
        bool findAssetDirectories(const string& dir) const;

    private:
        Project* mProject;
        string mPath;
        const static size_t BufferSize;
    };
}
