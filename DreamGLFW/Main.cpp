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

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <thread>
#include <memory>
#include <sstream>

#include "../DreamCore/deps/spdlog/spdlog.h"
#include "../DreamCore/deps/spdlog/sinks/stdout_color_sinks.h"

#include "../DreamCore/Scene/SceneRuntime.h"
#include "../DreamCore/Project/ProjectDirectory.h"
#include "../DreamCore/Project/ProjectDefinition.h"
#include "../DreamCore/Project/ProjectRuntime.h"
#include "../DreamCore/Project/Project.h"
#include "Window/GLFWWindowComponent.h"

#define MINIMUM_ARGUMENTS 3

#ifdef WIN32
#include <GL/glew.h>
#endif

using namespace std;
using namespace Dream;
using namespace DreamGLFW;


shared_ptr<spdlog::logger> _log = spdlog::stdout_color_mt("Main");

void run(int,char**);
Project* openProject(string);
void handleSceneInput(SceneRuntime*);

bool MainLoopDone = false;
ProjectDirectory projectDirectory;
GLFWWindowComponent windowComponent;

int
main
(int argc,char** argv)
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S|%n|%l] %v");


    if(argc < 2)
    {
        _log->error("No Project Argument.");
        return 1;
    }

    spdlog::set_level(spdlog::level::off);
    run(argc,argv);


    return 0;
}

Project*
openProject
(string dir)
{
    _log->debug("Opening project {}",dir);
    auto project = projectDirectory.openFromDirectory(dir);
    if(project)
    {
        project->setWindowComponent(&windowComponent);
        project->createProjectRuntime();
        return project;
    }
    return nullptr;
}

void
run
(int argc, char** argv)
{
    _log->trace("Starting...");

    if(!windowComponent.init())
    {
        _log->error("Could not initialise window component");
        return;
    }

    Project* project = openProject(argv[1]);
    ProjectRuntime* projectRuntime = nullptr;
    ProjectDefinition* projectDefinition = nullptr;
    SceneDefinition* startupSceneDefinition = nullptr;
    SceneRuntime* activeSceneRuntime = nullptr;

    if (project)
    {
        projectRuntime = project->getRuntime();
        projectDefinition = project->getDefinition();

        if (projectDefinition)
        {
            startupSceneDefinition = projectDefinition->getStartupSceneDefinition();
        }

        if (startupSceneDefinition && projectRuntime)
        {
            activeSceneRuntime = new SceneRuntime(startupSceneDefinition,projectRuntime);
            if(activeSceneRuntime->useDefinition())
            {
                projectRuntime->addSceneRuntime(activeSceneRuntime);
                projectRuntime->setSceneRuntimeAsActive(activeSceneRuntime->getUuid());
            }
            else
            {
                _log->error("Unable to use startup scene runtime");
                delete activeSceneRuntime;
                activeSceneRuntime = nullptr;
            }
        }
    }

    // Run the project
    while (!MainLoopDone)
    {
        if (windowComponent.shouldClose())
        {
            MainLoopDone = true;
        }

        if(activeSceneRuntime)
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            handleSceneInput(activeSceneRuntime);
            projectRuntime->updateAll();
            windowComponent.swapBuffers();
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            windowComponent.updateComponent(nullptr);
        }
    }

    spdlog::set_level(spdlog::level::trace);
    _log->info("Run is done. Cleaning up");

    if (activeSceneRuntime)
    {
        projectRuntime->destructSceneRuntime(activeSceneRuntime);
    }
}

void
handleSceneInput
(SceneRuntime* sRunt)
{
    /*
    auto pRunt = project->getRuntime();
    if (pRunt)
    {
        auto inputComp = pRunt->getInputComponent();
        if (inputComp)
        {
            MouseState& ms = inputComp->getMouseState();
            KeyboardState& ks = inputComp->getKeyboardState();
            JoystickState& js = inputComp->getJoystickState();

            // Mouse
            memcpy(ms.ButtonsDown, io.MouseDown, sizeof(bool)*5);
            ms.PosX = io.MousePos.x;
            ms.PosY = io.MousePos.y;
            ms.ScrollX = io.MouseWheelH;
            ms.ScrollY = io.MouseWheel;

            // Keys
            memcpy(ks.KeysDown, io.KeysDown, sizeof(bool)*512);

            // Joystick
            for (int id=GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; id++)
            {

                if (glfwJoystickPresent(id))
                {
                    js.Name = glfwGetJoystickName(id);
                    int numAxis, numButtons;
                    const float* axisData = glfwGetJoystickAxes(id,&numAxis);
                    const unsigned char* buttonData = glfwGetJoystickButtons(id, &numButtons);
                    if (axisData != nullptr)
                    {
                        js.AxisCount = numAxis;
                        memcpy(&js.AxisData[0],axisData,sizeof(float)*numAxis);
                    }
                    else
                    {
                        js.AxisCount = 0;
                    }
                    if (buttonData != nullptr)
                    {
                        js.ButtonCount = numButtons;
                        memcpy(&js.ButtonData[0],buttonData,sizeof(unsigned char)*numButtons);
                    }
                    else
                    {
                        js.ButtonCount = 0;
                    }
                }
            }
        }
    }
    */
}
