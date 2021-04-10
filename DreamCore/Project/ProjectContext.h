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
