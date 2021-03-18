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

#include <memory>
#include <string>
#include <vector>
#include <json.hpp>

using std::string;
using std::shared_ptr;
using std::vector;
using std::shared_ptr;
using nlohmann::json;
using std::enable_shared_from_this;
using std::weak_ptr;

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

    class Project : public enable_shared_from_this<Project>
    {
    public:
        Project(const weak_ptr<ProjectDirectory>& dir, const weak_ptr<StorageManager>& fm);
        ~Project();

        weak_ptr<ProjectRuntime> getRuntime() const;
        weak_ptr<ProjectRuntime> createProjectRuntime();
        bool hasProjectRuntime() const;
        void resetProjectRuntime();

        weak_ptr<ProjectDefinition> getDefinition() const;
        void setDefinition(const shared_ptr<ProjectDefinition>& definition);
        bool hasProjectDefinition() const;

        weak_ptr<AssetDefinition> getAssetDefinitionByUuid(UuidType uuid) const;
        void setWindowComponent(const shared_ptr<WindowComponent>& windowComponent);
        void setStorageManager(const shared_ptr<StorageManager>& storageManager);
        void setAudioComponent(const shared_ptr<AudioComponent>& audioComponent);

        weak_ptr<ProjectDirectory> getDirectory() const;

    private:
        shared_ptr<ProjectDirectory> mDirectory;
        shared_ptr<ProjectDefinition> mDefinition;
        shared_ptr<ProjectRuntime> mRuntime;
        shared_ptr<WindowComponent> mWindowComponent;
        shared_ptr<AudioComponent> mAudioComponent;
        shared_ptr<StorageManager> mStorageManager;
    };
}
