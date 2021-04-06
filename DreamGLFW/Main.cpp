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
#include "DefaultPrintListener.h"

#define MINIMUM_ARGUMENTS 3

// Using

using std::stoi;
using octronic::dream::ProjectContext;
using octronic::dream::WindowComponent;
using octronic::dream::MouseState;
using octronic::dream::KeyboardState;
using octronic::dream::JoystickState;
using octronic::dream::StorageManager;
using octronic::dream::open_al::OpenALAudioComponent;
using octronic::dream::glfw::GLFWWindowComponent;
using octronic::dream::glfw::DefaultPrintListener;

// Global variables

string _option_project_dir;
string _option_logLevel = "off";
int    _option_width = 0;
int    _option_height = 0;

// Global Functions

void parseArguments(int argc, char** argv)
{
  if(argc < 2)
  {
    for (int i = 0; i < argc; i++)
    {
      LOG_ERROR("DreamGLFW: Arg {}: {}", i, argv[i]);
    }
    LOG_ERROR("DreamGLFW: No Project Argument.");
    exit(1);
  }

  for (int i=0; i<argc; i++)
  {
    if (string(argv[i]) == "-l")
    {
      if (argc >= i+1)
      {
        _option_logLevel = string(argv[i+1]);
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
        _option_project_dir = string(argv[i+1]);
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
        _option_width = stoi(argv[i+1]);
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
        _option_height = stoi(argv[i+1]);
      }
      else
      {
        LOG_ERROR("Main: Height argument not found");
      }
    }
  }
}

void setupLogger()
{
  LOG_LEVEL(spdlog::level::from_str(_option_logLevel));
  LOG_ERROR("Main: Using log level {}", _option_logLevel);
  LOG_PATTERN("[%H:%M:%S]%l: %v");
}

void handleInput(ProjectContext(& context))
{
  auto& projectRuntimeOpt = context.getProjectRuntime();
  if (projectRuntimeOpt)
  {
    auto& projectRuntime = projectRuntimeOpt.value();
    auto& inputComp = projectRuntime.getInputComponent();
    // Keyboard
    auto& ks = inputComp.getKeyboardState();
    ks.setKeysPressed(GLFWWindowComponent::KeysDown, 512);
    inputComp.setKeyboardState(ks);

    // Mouse
    auto& ms = inputComp.getMouseState();
    ms.setButtonsPressed(GLFWWindowComponent::MouseButtonsDown, 5);
    ms.setPosX(GLFWWindowComponent::MousePosX);
    ms.setPosY( GLFWWindowComponent::MousePosY);
    ms.setScrollX( GLFWWindowComponent::MouseWheelH);
    ms.setScrollY(GLFWWindowComponent::MouseWheel);
    inputComp.setMouseState(ms);

    // Joystick
    auto& js = inputComp.getJoystickState();
    int joystickCount = 0;
    for (int id=GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; id++)
    {
      if (glfwJoystickPresent(id))
      {
        joystickCount++;
        js.setName(glfwGetJoystickName(id));
        int numAxis, numButtons;
        const float* axisData = glfwGetJoystickAxes(id,&numAxis);
        const unsigned char* buttonData = glfwGetJoystickButtons(id, &numButtons);
        if (axisData != nullptr)
        {
          js.setAxisCount(numAxis);
          memcpy((void*)js.getAxisDataPointer(),axisData,sizeof(float)*numAxis);
        }
        else
        {
          js.setAxisCount(0);
        }
        if (buttonData != nullptr)
        {
          js.setButtonCount(numButtons);
          memcpy((void*)js.getButtonDataPointer(),buttonData,sizeof(unsigned char)*numButtons);
        }
        else
        {
          js.setButtonCount(0);
        }
      }
    }
    inputComp.setJoystickState(js);
    inputComp.setJoystickCount(joystickCount);
  }
}

// Entry Point

int main(int argc,char** argv)
{
  parseArguments(argc, argv);
  setupLogger();

  LOG_INFO("DreamGLFW: Starting...");

  StorageManager storageMan;

  OpenALAudioComponent audioComp;
  if(!audioComp.init()) return 1;

  GLFWWindowComponent windowComp;
  windowComp.setWidth(_option_width);
  windowComp.setHeight(_option_height);
  if(!windowComp.init()) return 2;
  DefaultPrintListener pl;

  ProjectContext context(windowComp,audioComp,storageMan,_option_project_dir);
  context.addPrintListener(pl);

  if (context.openFromPath())
  {
    auto& wc = context.getWindowComponent();
    while (!wc.shouldClose())
    {
      wc.updateWindow();
      handleInput(context);
      context.step();
      wc.swapBuffers();
    }
  }
}



