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

#include "Common/Uuid.h"


#include <string>
#include <vector>
#include <json.hpp>

using std::string;
using std::shared_ptr;
using std::vector;
using nlohmann::json;

namespace octronic::dream
{
    class ArgumentParser;
    class File;
    class ProjectRuntime;
    class ProjectDefinition;
    class AssetDefinition;
    class WindowComponent;
    class ProjectDirectory;
    class StorageManager;
    class AudioComponent;

    class Project
    {


        // Public Methods
    public:
        Project(ProjectDirectory* dir, StorageManager* fm);
        ~Project();

        ProjectRuntime* getRuntime() const;
        ProjectRuntime* createProjectRuntime();
        bool hasProjectRuntime() const;
        void resetProjectRuntime();

        ProjectDefinition* getDefinition() const;
        void setDefinition(ProjectDefinition* definition);
        bool hasProjectDefinition() const;

        AssetDefinition* getAssetDefinitionByUuid(UuidType uuid) const;
        void setWindowComponent(WindowComponent* windowComponent);
        void setStorageManager(StorageManager* storageManager);
        void setAudioComponent(AudioComponent* audioComponent);

        ProjectDirectory* getDirectory() const;
            // Variables
    private:
        ProjectDirectory* mDirectory;
        ProjectDefinition* mDefinition;
        ProjectRuntime* mRuntime;
        WindowComponent* mWindowComponent;
        AudioComponent* mAudioComponent;
        StorageManager* mStorageManager;
    };
}
