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

#include "Project/ProjectRuntime.h"
#include "Project/ProjectDirectory.h"
#include "Project/ProjectDefinition.h"
#include "Storage/StorageManager.h"

#include <memory>
#include <string>
#include <vector>
#include <json.hpp>

using std::string;
using std::vector;
using nlohmann::json;

namespace octronic::dream
{
  class ProjectRuntime;
  class ProjectDefinition;
  class ProjectDirectory;
  class WindowComponent;
  class AudioComponent;
  class ScriptPrintListener;

  /**
   * @brief The ProjectContext class owns the objects required to run a Project.
   * These include
   *     ProjectDirectory
   *     ProjectDefinition
   *     ProjectRuntime
   *
   *     A unique ProjectContext should be used for each Project.
   */
  class ProjectContext
  {
  public:
    ProjectContext(WindowComponent& wc, AudioComponent& ac,
                   StorageManager& sm, const string& base_dir);

    ProjectContext(ProjectContext&&) = default;
    ProjectContext& operator=(ProjectContext&&) = default;

    ProjectContext(const ProjectContext&) = delete;
    ProjectContext& operator=(const ProjectContext&) = delete;

    bool openFromPath();
    bool newProject(const string& baseDir);

    WindowComponent& getWindowComponent() const;
    AudioComponent& getAudioComponent() const;
    StorageManager& getStorageManager() const;

    optional<ProjectDirectory>&  getProjectDirectory();
    optional<ProjectDefinition>& getProjectDefinition();
    optional<ProjectRuntime>&    getProjectRuntime();
    ProjectRuntime& createProjectRuntime();

    void setProjectPath(const string& path);
    void addPrintListener(ScriptPrintListener& pl);
    bool getShouldClose() const;

    void loadProject();
    void step();

  private:
    reference_wrapper<WindowComponent> mWindowComponent;
    reference_wrapper<AudioComponent>  mAudioComponent;
    reference_wrapper<StorageManager>  mStorageManager;

    optional<ProjectDirectory>  mProjectDirectory;
    optional<ProjectDefinition> mProjectDefinition;
    optional<ProjectRuntime>    mProjectRuntime;
  };
}
