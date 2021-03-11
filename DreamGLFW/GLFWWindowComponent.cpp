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


#include "GLFWWindowComponent.h"

using octronic::dream::WindowComponent;

namespace octronic::dream::glfw
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
        LOG_ERROR("GLFW Error: Number {}\nMessage: {}" , _errno, errmsg);
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
         mDPIScaleY(1.0f),
         mLastWidth(0.f),
    	 mLastHeight(0.f)
    {
        LOG_INFO("GLFWWindowComponent: Constructing" );
        mName = "Dream";
    }

    GLFWWindowComponent::~GLFWWindowComponent
    ()
    {
        LOG_INFO("GLFWWindowComponent: Destructing" );
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
    GLFWWindowComponent::bindFrameBuffer
    ()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        GLCheckError();
    }

    GLuint
    GLFWWindowComponent::getFrameBuffer
    () const
    {
        return 0;
    }

    GLuint
    GLFWWindowComponent::getDepthBuffer()
    const
    {
        return 0;
    }

    bool
    GLFWWindowComponent::initGLFW
    ()
    {
        LOG_DEBUG("GLFWWindowComponent: Initialising GLFW");
        glfwSetErrorCallback(GLFWErrorCallback);
        /* Initialize the library */
        if (!glfwInit())
        {
            LOG_ERROR("GLFWWindowComponent: FAILED @ Initialising GLFW");
            return false;
        }

        /* Create a windowed mode window and its OpenGL context */
        //glfwWindowHint(GLFW_SAMPLES, 4);
        #ifdef WIN32
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        //glfwWindowHint( GLFW_DOUBLEBUFFER,GL_FALSE );
        #endif
        mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr,nullptr);

        if (mWindow == nullptr)
        {
            LOG_ERROR("GLFWWindowComponent: FAILED @ Make Window");
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(mWindow);
        // Register Callbacks
        glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
        glfwSetKeyCallback(mWindow, KeyboardInputCallback);
        glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);
        glfwSetCursorPosCallback(mWindow,CursorPositionCallback);
        glfwSetScrollCallback(mWindow,MouseWheelCallback);

        glfwSwapInterval(0);
        //glfwGetMonitorContentScale(glfwGetPrimaryMonitor(),mDPIScaleX,mDPIScaleY); Requires GLFW >=3.3
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        LOG_DEBUG("GLFWWindowComponent: Queried Framebuffer size as {}x{}",mWidth,mHeight);
        return true;
    }

    bool
    GLFWWindowComponent::initGL
    ()
    {
        LOG_DEBUG("GLFWWindowComponent: Initialising GLFW::OpenGL");

        if(!gladLoadGL())
		{
			LOG_ERROR("GLFWWindowComponent: Window: Error initialising GLAD!\n");
			return false;
		}

        GLCheckError();

        LOG_DEBUG(
            "GLFWWindowComponent: OpenGL Version {}, Shader Version {}",
            glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION)
        );
        return true;
    }

    void
    GLFWWindowComponent::updateWindow
    ()
    {
        glfwPollEvents();

        if(glfwWindowShouldClose(mWindow))
        {
            setShouldClose(true);
            LOG_ERROR("GLFWWindowComponent: Window should close");
        }

        if (WindowSizeChanged)
        {
            getCurrentDimensions();
            WindowSizeChanged = false;
        }
    }

    void GLFWWindowComponent::pushTasks()
    {

    }

    void
    GLFWWindowComponent::getCurrentDimensions
    ()
    {
        mLastWidth = mWidth;
        mLastHeight = mHeight;
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        LOG_TRACE("GLFWWindowComponent: Framebuffer Size Changed: {}x{}", mWidth ,mHeight);
        if (mLastWidth != mWidth || mLastHeight != mHeight) setWindowSizeChangedFlag(true);
    }

    void
    GLFWWindowComponent::swapBuffers
    ()
    {
        if (mWindow != nullptr) glfwSwapBuffers(mWindow);
    }

    // These members are static so they can be accessed by GLFW Callbacks;
    bool  GLFWWindowComponent::WindowSizeChanged = false;
    bool  GLFWWindowComponent::MouseButtonsDown[5] = {false};
    float GLFWWindowComponent::MousePosX = 0.0f;
    float GLFWWindowComponent::MousePosY = 0.0f;
    float GLFWWindowComponent::MouseWheel = 0.0f;
    float GLFWWindowComponent::MouseWheelH = 0.0f;
    bool  GLFWWindowComponent::KeysDown[512] = {false};
}

