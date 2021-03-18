/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include <thread>
#include <memory>
#include <sstream>

#include <DreamCore.h>
#include <OpenALAudioComponent.h>
#include "GLFWWindowComponent.h"

#define MINIMUM_ARGUMENTS 3

using std::shared_ptr;
using std::weak_ptr;
using octronic::dream::Project;
using octronic::dream::WindowComponent;
using octronic::dream::ProjectDirectory;
using octronic::dream::ProjectDefinition;
using octronic::dream::ProjectRuntime;
using octronic::dream::ScriptComponent;
using octronic::dream::SceneDefinition;
using octronic::dream::SceneRuntime;
using octronic::dream::MouseState;
using octronic::dream::KeyboardState;
using octronic::dream::JoystickState;
using octronic::dream::GraphicsComponent;
using octronic::dream::glfw::GLFWWindowComponent;
using octronic::dream::StorageManager;
using octronic::dream::Directory;
using octronic::dream::open_al::OpenALAudioComponent;
using octronic::dream::ScriptPrintListener;
using octronic::dream::FontRuntime;
using octronic::dream::SceneState;
using std::stoi;

class DefaultPrintListener : public ScriptPrintListener
{
public:
  DefaultPrintListener() {}
  ~DefaultPrintListener() {}

  void onPrint(const string& str) override
  {
    LOG_ERROR("LUA PrintListener: {}", str);
  }

};


void run(int,char**);
void handleSceneInput(const shared_ptr<Project>&);

int main(int argc,char** argv)
{
  string dir;
  string logLevel = "off";
  int width = 0;
  int height = 0;

  for (int i=0; i<argc; i++)
  {
    if (string(argv[i]) == "-l")
    {
      if (argc >= i+1)
      {
        logLevel = string(argv[i+1]);
      }
      else
      {
        LOG_ERROR("Main: Log Level argument not found");
      }

    }
    else if (string(argv[i]) == "-p")
    {
      if (argc >= i+1)
      {
        dir = string(argv[i+1]);
      }
      else
      {
        LOG_ERROR("Main: Width argument not found");
      }
    }
    else if (string(argv[i]) == "-w")
    {
      if (argc >= i+1)
      {
        width = stoi(argv[i+1]);
      }
      else
      {
        LOG_ERROR("Main: Width argument not found");
      }
    }
    else if (string(argv[i]) == "-h")
    {
      if (argc >= i+1)
      {
        height = stoi(argv[i+1]);
      }
      else
      {
        LOG_ERROR("Main: Height argument not found");
      }
    }
  }

  LOG_LEVEL(spdlog::level::from_str(logLevel));
  LOG_ERROR("Main: Using log level {}", logLevel);
  LOG_PATTERN("[%H:%M:%S]%l: %v");

  if(argc < 2)
  {
    for (int i = 0; i < argc; i++)
    {
      LOG_ERROR("DreamGLFW: Arg {}: {}", i, argv[i]);
    }
    LOG_ERROR("DreamGLFW: No Project Argument.");
    exit(1);
  }

  bool MainLoopDone = false;

  LOG_INFO("DreamGLFW: Starting...");

  auto audioComponent = make_shared<OpenALAudioComponent>();
  if (!audioComponent->init())
  {
    LOG_ERROR("DreamGLFW: Unable to init audio component");
    exit(1);
  }

  auto  windowComponent = make_shared<GLFWWindowComponent>();
  windowComponent->setWidth(width);
  windowComponent->setHeight(height);
  if(!windowComponent->init())
  {
    LOG_ERROR("DreamGLFW: Could not initialise window component");
    exit(1);
  }

  auto lua_pl = make_shared<DefaultPrintListener>();
  auto fileManager = make_shared<StorageManager>();
  shared_ptr<Project> project;
  auto  projectDirectory = make_shared<ProjectDirectory>(fileManager);
  ScriptComponent::AddPrintListener(lua_pl);
  FontRuntime::InitFreetypeLibrary();
  auto d = fileManager->openDirectory(dir);

  LOG_DEBUG("DreamGLFW: Opening project {}",dir);
  project = projectDirectory->openFromDirectory(d);

  if(!project)
  {
    LOG_ERROR("DreamGLFW: ");
    exit(1);
  }

  project->setAudioComponent(audioComponent);
  project->setWindowComponent(windowComponent);
  project->createProjectRuntime();

  auto projectRuntime = project->getRuntime().lock();
  auto projectDefinition = project->getDefinition().lock();

  if (!projectDefinition)
  {
    LOG_ERROR("DreamGLFW: ");
    exit(1);
  }

  auto startupSceneDefinition = projectDefinition->getStartupSceneDefinition().lock();
  shared_ptr<SceneRuntime> startupSceneRuntime;

  if (projectRuntime && startupSceneDefinition)
  {
    startupSceneRuntime = make_shared<SceneRuntime>(projectRuntime, startupSceneDefinition);
    if(startupSceneRuntime->init())
    {
      projectRuntime->addSceneRuntime(startupSceneRuntime);
    }
  }

  // Run the project
  while (!MainLoopDone)
  {
    windowComponent->updateWindow();

    if (windowComponent->shouldClose())
    {
      MainLoopDone = true;
    }

    if (project == nullptr) continue;
    if (projectRuntime == nullptr) continue;

    projectRuntime->step();

    handleSceneInput(project);

    if (startupSceneRuntime && startupSceneRuntime->hasState(SceneState::SCENE_STATE_LOADED))
    {
      projectRuntime->setActiveSceneRuntime(startupSceneRuntime->getUuid());
      startupSceneRuntime->setState(SceneState::SCENE_STATE_ACTIVE);
    }
    windowComponent->swapBuffers();
  }

  LOG_INFO("DreamGLFW: Run is done. Cleaning up");

  if (startupSceneRuntime)
  {
    projectRuntime->destructSceneRuntime(startupSceneRuntime);
  }
  FontRuntime::CleanupFreetypeLibrary();
}

void handleSceneInput(const shared_ptr<Project>& project)
{
  auto pRunt = project->getRuntime().lock();

  if (pRunt)
  {
    auto inputComp = pRunt->getInputComponent().lock();
    if (inputComp)
    {
      // Keyboard
      auto ks = inputComp->getKeyboardState();
      ks->setKeysPressed(GLFWWindowComponent::KeysDown, 512);
      inputComp->setKeyboardState(ks);

      // Mouse
      auto ms = inputComp->getMouseState();
      ms->setButtonsPressed(GLFWWindowComponent::MouseButtonsDown, 5);
      ms->setPosX(GLFWWindowComponent::MousePosX);
      ms->setPosY( GLFWWindowComponent::MousePosY);
      ms->setScrollX( GLFWWindowComponent::MouseWheelH);
      ms->setScrollY(GLFWWindowComponent::MouseWheel);
      inputComp->setMouseState(ms);

      // Joystick
      auto js = inputComp->getJoystickState();
      int joystickCount = 0;
      for (int id=GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; id++)
      {
        if (glfwJoystickPresent(id))
        {
          joystickCount++;
          js->setName(glfwGetJoystickName(id));
          int numAxis, numButtons;
          const float* axisData = glfwGetJoystickAxes(id,&numAxis);
          const unsigned char* buttonData = glfwGetJoystickButtons(id, &numButtons);
          if (axisData != nullptr)
          {
            js->setAxisCount(numAxis);
            memcpy((void*)js->getAxisDataPointer(),axisData,sizeof(float)*numAxis);
          }
          else
          {
            js->setAxisCount(0);
          }
          if (buttonData != nullptr)
          {
            js->setButtonCount(numButtons);
            memcpy((void*)js->getButtonDataPointer(),buttonData,sizeof(unsigned char)*numButtons);
          }
          else
          {
            js->setButtonCount(0);
          }
        }
      }
      inputComp->setJoystickState(js);
      inputComp->setJoystickCount(joystickCount);
    }
  }
}
