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
#include <GL/glew.h>
#include "GLFWWindowComponent.h"
#include "../../DreamCore/Scene/SceneRuntime.h"

using namespace Dream;

namespace DreamGLFW
{
    void
    FramebufferSizeCallback
    (GLFWwindow*, int, int)
    {
        GLFWWindowComponent::WindowSizeChanged = true;
    }

    void
    GLFWErrorCallback
    (int _errno, const char* errmsg)
    {
        cout << "GLFW Error: Number " << _errno << "\nMessage: " << errmsg << endl;
    }

    void
    KeyboardInputCallback
    (GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        GLFWWindowComponent::KeysDown[key] = action == GLFW_PRESS || action == GLFW_REPEAT;
    }

    void
    MouseButtonCallback
    (GLFWwindow* window, int button, int action, int mods)
    {
       GLFWWindowComponent::MouseButtonsDown[button] = action == GLFW_PRESS;
    }

    void
    CursorPositionCallback
    (GLFWwindow* window, double xpos, double ypos)
    {
        GLFWWindowComponent::MousePosX = xpos;
        GLFWWindowComponent::MousePosY = ypos;
    }

    void
    MouseWheelCallback
    (GLFWwindow* , double xoff, double yoff)
    {
        GLFWWindowComponent::MouseWheelH = xoff;
        GLFWWindowComponent::MouseWheel = yoff;
    }

    GLFWWindowComponent::GLFWWindowComponent
    () : WindowComponent(),
         mDPIScaleX(1.0f),
         mDPIScaleY(1.0f)
    {
#ifdef DREAM_LOG
        setLogClassName("GLFWWindowComponent");
        auto log = getLog();
        log->info("Constructing" );
#endif
        mName = "Dream";
    }

    GLFWWindowComponent::~GLFWWindowComponent
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->info("Destructing" );
#endif
        glfwTerminate();
        mWindow = nullptr;
    }

    GLFWwindow*
    GLFWWindowComponent::getGlfwWindow
    ()
    {
       return mWindow;
    }

    bool
    GLFWWindowComponent::init
    ()
    {
        if (!initGLFW())
        {
            return false;
        }

        if (!initGL())
        {
            return false;
        }

        return true;
    }

    void
    GLFWWindowComponent::bindDefaultFrameBuffer
    ()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
#ifdef DREAM_LOG
        checkGLError();
#endif
    }

    bool
    GLFWWindowComponent::initGLFW
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Initialising GLFW");
#endif
        glfwSetErrorCallback(GLFWErrorCallback);
        /* Initialize the library */
        if (!glfwInit())
        {
#ifdef DREAM_LOG
            log->error("FAILED @ Initialising GLFW");
#endif
            return false;
        }

        /* Create a windowed mode window and its OpenGL context */
        glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef WIN32
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#endif
        mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr,nullptr);

        if (mWindow == nullptr)
        {
#ifdef DREAM_LOG
            log->error("FAILED @ Make Window");
#endif
            glfwTerminate();
            return false;
        }

        // Register Callbacks
        glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
        glfwSetKeyCallback(mWindow, KeyboardInputCallback);
        glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);
        glfwSetCursorPosCallback(mWindow,CursorPositionCallback);
        glfwSetScrollCallback(mWindow,MouseWheelCallback);

        glfwSwapInterval(0);
        //glfwGetMonitorContentScale(glfwGetPrimaryMonitor(),mDPIScaleX,mDPIScaleY); Requires GLFW >=3.3
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
#ifdef DREAM_LOG
        log->debug("Queried Framebuffer size as {}x{}",mWidth,mHeight);
#endif
        return true;
    }

    bool
    GLFWWindowComponent::initGL
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Initialising GLFW::OpenGL");
#endif
        glfwMakeContextCurrent(mWindow);

        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();

        if (glewInitResult != GLEW_OK)
        {
#ifdef DREAM_LOG
            log->error("GLEW failed to initialise");
#endif
            return false;
        }

#ifdef DREAM_LOG
        checkGLError();

        log->debug(
            "OpenGL Version {}, Shader Version {}",
            glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION)
        );
#endif
        return true;
    }

    void
    GLFWWindowComponent::updateComponent
    (SceneRuntime* sr)
    {
#ifdef DREAM_LOG
        auto log = getLog();
#endif

        glfwPollEvents();

        if(glfwWindowShouldClose(mWindow))
        {
            if (sr != nullptr)
            {
                sr->setState(Dream::SCENE_STATE_TO_DESTROY);
            }
            setShouldClose(true);
#ifdef DREAM_LOG
            log->error("Window should close");
#endif
        }

        if (WindowSizeChanged)
        {
            getCurrentDimensions();
            WindowSizeChanged = false;
        }
    }

    void
    GLFWWindowComponent::getCurrentDimensions
    ()
    {
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
#ifdef DREAM_LOG
        auto log = getLog();
        log->error("Framebuffer Size Changed: {}x{}", mWidth ,mHeight);
#endif
        mSizeHasChanged = true;
    }

    void
    GLFWWindowComponent::swapBuffers
    ()
    {
        if (mWindow != nullptr)
        {
            glfwSwapBuffers(mWindow);
        }
    }

    int
    GLFWWindowComponent::FPS
    ()
    {
        LastTime = CurrentTime;
        CurrentTime = glfwGetTime();
        Frames = 1.0/(CurrentTime-LastTime);
        return Frames;
    }

    bool  GLFWWindowComponent::WindowSizeChanged = false;
    bool  GLFWWindowComponent::MouseButtonsDown[5] = {false};
    float GLFWWindowComponent::MousePosX = 0.0f;
    float GLFWWindowComponent::MousePosY = 0.0f;
    float GLFWWindowComponent::MouseWheel = 0.0f;
    float GLFWWindowComponent::MouseWheelH = 0.0f;
    bool  GLFWWindowComponent::KeysDown[512] = {false};
    float GLFWWindowComponent::LastTime = 0;
    float GLFWWindowComponent::CurrentTime = 0;
    int   GLFWWindowComponent::Frames = 0;

}

