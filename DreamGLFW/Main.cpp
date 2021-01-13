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

#include <iostream>
#include <thread>
#include <memory>
#include <sstream>

#include <DreamCore.h>
#include "Window/GLFWWindowComponent.h"

#define MINIMUM_ARGUMENTS 3

using std::shared_ptr;
using Dream::Project;
using Dream::WindowComponent;
using Dream::ProjectDirectory;
using Dream::ProjectDefinition;
using Dream::ProjectRuntime;
using Dream::SceneDefinition;
using Dream::SceneRuntime;
using Dream::MouseState;
using Dream::KeyboardState;
using Dream::JoystickState;
using DreamGLFW::GLFWWindowComponent;

void run(int,char**);
Project* openProject(ProjectDirectory&, string, WindowComponent&);
void handleSceneInput(Project*);

int
main
(int argc,char** argv)
{
    string logLevel = "trace";
    for (int i=0; i<argc; i++)
    {
        if (string(argv[i]) == "-l")
        {
            if (argc >= i+1)
            {
                logLevel = string(argv[i+1]);
            }
        }
    }
    cout << "Using log level " << logLevel << endl;
    LOG_LEVEL(spdlog::level::from_str(logLevel));
    //spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
    spdlog::set_pattern("%H:%M:%S (%l) %n: %v");

    if(argc < 2)
    {
        for (int i = 0; i < argc; i++)
        {
            LOG_ERROR("DreamGLFW: Arg {}: {}", i, argv[i]);
        }
        LOG_ERROR("DreamGLFW: No Project Argument.");
        return 1;
    }

    run(argc,argv);

    return 0;
}

Project* openProject(ProjectDirectory& projectDirectory, string dir, WindowComponent& windowComponent)
{
    LOG_DEBUG("DreamGLFW: Opening project {}",dir);
    auto project = projectDirectory.openFromDirectory(dir);
    if(project)
    {
        project->setWindowComponent(&windowComponent);
        project->createProjectRuntime();
        return project;
    }
    return nullptr;
}

void run(int argc, char** argv)
{
    bool MainLoopDone = false;
    ProjectDirectory projectDirectory;
    GLFWWindowComponent windowComponent;
    ProjectRuntime* projectRuntime = nullptr;
    ProjectDefinition* projectDefinition = nullptr;
    SceneDefinition* startupSceneDefinition = nullptr;
    SceneRuntime* activeSceneRuntime = nullptr;

    LOG_INFO("DreamGLFW: Starting...");

    if(!windowComponent.init())
    {
        LOG_ERROR("DreamGLFW: Could not initialise window component");
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
                LOG_ERROR("DreamGLFW: Unable to use startup scene runtime");
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

    LOG_INFO("DreamGLFW: Run is done. Cleaning up");

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
            inputComp->setJoystickCount(joystickCount);
        }
    }
}

