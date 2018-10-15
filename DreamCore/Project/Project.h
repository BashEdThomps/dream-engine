/*
* Dream::Project
*
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
#include <memory>
#include <vector>

#ifdef __APPLE__
    #include <nlohmann/json.hpp>
#else
    #include <json.hpp>
#endif

#include "../Common/DreamObject.h"

using std::string;
using std::shared_ptr;
using std::vector;
using nlohmann::json;

namespace Dream
{
    class ArgumentParser;
    class FileReader;
    class ProjectRuntime;
    class ProjectDefinition;
    class IAssetDefinition;
    class IWindowComponent;

    class Project :
        public DreamObject
    {
        // Variables
    private:
        ProjectDefinition* mDefinition;
        ProjectRuntime* mRuntime;
        IWindowComponent* mWindowComponent;
        string mProjectPath;

        // Public Methods
    public:
        Project(IWindowComponent* wc = nullptr);
        ~Project();

        ProjectRuntime* getProjectRuntime();
        ProjectDefinition* getProjectDefinition();

        bool openFromFileReader(string directory, FileReader &fileReader);
        bool openFromArgumentParser(ArgumentParser &parser);
        bool openFromDirectory(string directory);

        ProjectRuntime* createProjectRuntime();
        static ProjectDefinition* createNewProjectDefinition(string name = Constants::PROJECT_DEFAULT_NAME);

        bool hasProjectRuntime();
        void resetProjectRuntime();

        string getProjectPath();

        bool hasProjectDefinition();
        IAssetDefinition* getAssetDefinitionByUuid(string uuid);
    }; // End of Project

} // End of Dream
