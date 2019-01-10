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

#include "../Common/DreamObject.h"
#include "../deps/json/json.hpp"

using std::string;
using std::shared_ptr;
using std::vector;
using nlohmann::json;

namespace Dream
{
    class ArgumentParser;
    class File;
    class ProjectRuntime;
    class ProjectDefinition;
    class AssetDefinition;
    class WindowComponent;
    class ProjectDirectory;

    class Project :
        public DreamObject
    {
        // Variables
    private:
        ProjectDirectory* mDirectory;
        ProjectDefinition* mDefinition;
        ProjectRuntime* mRuntime;
        WindowComponent* mWindowComponent;

        // Public Methods
    public:
        Project(ProjectDirectory* dir);
        ~Project();

        ProjectRuntime* getRuntime();
        ProjectRuntime* createProjectRuntime();
        bool hasProjectRuntime();
        void resetProjectRuntime();

        ProjectDefinition* getDefinition() const;
        void setDefinition(ProjectDefinition* definition);
        bool hasProjectDefinition();

        AssetDefinition* getAssetDefinitionByUuid(uint32_t uuid);
        void setWindowComponent(WindowComponent* windowComponent);

        ProjectDirectory* getDirectory() const;
    };
}
