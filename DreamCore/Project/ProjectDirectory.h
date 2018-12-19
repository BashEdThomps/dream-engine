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

#include "../Common/DreamObject.h"
#include "../Utilities/ArgumentParser.h"
#include "../Utilities/File.h"

namespace Dream
{
    class Project;
    class AssetDefinition;

    class ProjectDirectory : public DreamObject
    {
    public:
        ProjectDirectory();
        ProjectDirectory(Project* project);
        ~ProjectDirectory() override;

        bool baseDirectoryExists();
        bool createBaseDirectory();
        bool createAllAssetDirectories();

        vector<char> readAssetData(AssetDefinition*, string format = "");
        bool writeAssetData(AssetDefinition*, vector<char>, string format = "");

        string getAssetAbsolutePath(string);
        string getAssetAbsolutePath(AssetDefinition*);
        string getAssetAbsolutePath(AssetDefinition*, string format);
        string getAssetDirectoryPath(AssetDefinition*);
        string getAssetTypeDirectory(AssetType type, string base = "");

        bool removeAssetDirectory(AssetDefinition*);

        string getProjectFilePath();
        bool assetTypeDirectoryExists(AssetType type);

        bool createAssetTypeDirectory(AssetType type);
        vector<string> cleanupAssetsDirectory();

        Project* newProject(string projectDir);
        Project* openFromFileReader(string directory, File &fileReader);
        Project* openFromArgumentParser(ArgumentParser &parser);
        Project* openFromDirectory(string directory);
        bool     saveProject();
        void     closeProject();

    protected:
        string findProjectFileInDirectory(string dir);
        bool directoryContainsProject(string dir);
        bool findAssetDirectories(string dir);

    private:
        Project* mProject;
        string mPath;
        const static size_t BufferSize;
    };
}
