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
#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#endif
#include <iostream>
#include <thread>
#include <memory>
#include <sstream>

#ifdef DREAM_LOG
#include <spdlog/spdlog.h>
    #ifdef WIN32
        #include <spdlog/sinks/stdout_color_sinks.h>
    #endif
    #ifdef __APPLE__
        #include <spdlog/sinks/stdout_color_sinks.h>
    #endif
    #ifdef __linux__
        #include <spdlog/sinks/stdout_sinks.h>
    #endif
#endif

#include "../DreamCore/Scene/SceneRuntime.h"
#include "../DreamCore/Components/Input/InputComponent.h"
#include "../DreamCore/Project/ProjectDirectory.h"
#include "../DreamCore/Project/ProjectDefinition.h"
#include "../DreamCore/Project/ProjectRuntime.h"
#include "../DreamCore/Project/Project.h"
#include "Window/GLFWWindowComponent.h"

#define MINIMUM_ARGUMENTS 3



using namespace std;
using namespace Dream;
using namespace DreamGLFW;

#ifdef DREAM_LOG
shared_ptr<spdlog::logger> _log = spdlog::stdout_color_mt("Main");
#endif

void run(int,char**);
Project* openProject(ProjectDirectory&, string, WindowComponent&);
void handleSceneInput(Project*);


int
main
(int argc,char** argv)
{
    #ifdef DREAM_LOG
    spdlog::set_level(spdlog::level::off);
    spdlog::set_pattern("%H:%M:%S (%l) %n %v");
    #endif

    if(argc < 2)
    {
        #ifdef DREAM_LOG
        for (int i = 0; i < argc; i++)
        {
            _log->error("Arg {}: {}", i, argv[i]);
        }
        _log->error("No Project Argument.");
        #endif
        return 1;
    }

    run(argc,argv);

    return 0;
}

Project*
openProject
(ProjectDirectory& projectDirectory, string dir, WindowComponent& windowComponent)
{
    #ifdef DREAM_LOG
    _log->debug("Opening project {}",dir);
    #endif
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
    bool MainLoopDone = false;
    ProjectDirectory projectDirectory;
    GLFWWindowComponent windowComponent;
    ProjectRuntime* projectRuntime = nullptr;
    ProjectDefinition* projectDefinition = nullptr;
    SceneDefinition* startupSceneDefinition = nullptr;
    SceneRuntime* activeSceneRuntime = nullptr;

    #ifdef DREAM_LOG
    _log->trace("Starting...");
    #endif

    if(!windowComponent.init())
    {
        #ifdef DREAM_LOG
        _log->error("Could not initialise window component");
        #endif
        return;
    }

    Project* project = openProject(projectDirectory, argv[1], windowComponent);

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
                #ifdef DREAM_LOG
                _log->error("Unable to use startup scene runtime");
                #endif
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
            handleSceneInput(project);
            projectRuntime->updateAll();
            windowComponent.swapBuffers();
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            windowComponent.updateWindow(nullptr);
        }

        //std::cout << "FPS: " << GLFWWindowComponent::FPS() << std::endl;
    }

    #ifdef DREAM_LOG
    _log->info("Run is done. Cleaning up");
    #endif

    if (activeSceneRuntime)
    {
        projectRuntime->destructSceneRuntime(activeSceneRuntime);
    }
}

void
handleSceneInput
(Project* project)
{
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
            memcpy(ms.ButtonsDown, GLFWWindowComponent::MouseButtonsDown, sizeof(bool)*5);
            ms.PosX = GLFWWindowComponent::MousePosX;
            ms.PosY = GLFWWindowComponent::MousePosY;
            ms.ScrollX = GLFWWindowComponent::MouseWheelH;
            ms.ScrollY = GLFWWindowComponent::MouseWheel;

            memcpy(ks.KeysDown, GLFWWindowComponent::KeysDown, sizeof(bool)*512);

            // Joystick
            for (int id=GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; id++)
            {

                if (glfwJoystickPresent(id))
                {
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
        }
    }
}

