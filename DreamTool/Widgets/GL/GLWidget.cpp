//
//  GLWidget.cpp
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#include "GLWidget.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../DTState.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Components/Graphics/GraphicsComponent.h"
#include "../../../DreamCore/Components/Graphics/Shader/ShaderRuntime.h"
#include "../../../DreamCore/Scene/SceneRuntime.h"

namespace DreamTool
{
    GLWidget::GLWidget
    (DTState* project, bool visible)
        : DTWidget (project, visible),
          mModelMatrix(mat4(1.0f)),
          mViewMatrix(mat4(1.0f)),
          mProjectionMatrix(mat4(1.0f)),
          mVao(0),
          mVbo(0),
          mShaderProgram(0)
    {
#ifdef DREAM_LOG
        setLogClassName("GLWidget");
#endif
    }

    void GLWidget::init()
    {
        setShader();
        initShader();
        initVaoVbo();
    }

    GLWidget::~GLWidget()
    {

#ifdef DREAM_LOG
        auto log=getLog();
        log->debug("Destroying");
#endif
        if (mVao > 0)
        {
            glDeleteVertexArrays(1,&mVao);

#ifdef DREAM_LOG
            checkGLError();
#endif
        }
        if (mVbo > 0)
        {
            glDeleteBuffers(1,&mVbo);

#ifdef DREAM_LOG
            checkGLError();
#endif
        }
        if (mShaderProgram > 0)
        {
            glDeleteProgram(mShaderProgram);

#ifdef DREAM_LOG
            checkGLError();
#endif
        }
    }

    void
    GLWidget::initVaoVbo
    ()
    {
        glGenVertexArrays(1,&mVao);

#ifdef DREAM_LOG
        checkGLError();
#endif
        glBindVertexArray(mVao);
        ShaderRuntime::CurrentVAO = mVao;

#ifdef DREAM_LOG
        checkGLError();
#endif

        glGenBuffers(1,&mVbo);

#ifdef DREAM_LOG
        checkGLError();
#endif

        glBindBuffer(GL_ARRAY_BUFFER,mVbo);
        ShaderRuntime::CurrentVBO = mVbo;

        // Vertex Positions
        glEnableVertexAttribArray(0);

#ifdef DREAM_LOG
        checkGLError();
#endif
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(GLWidgetVertex)),
            static_cast<GLvoid*>(0)
        );

#ifdef DREAM_LOG
        checkGLError();
#endif

        // Vertex Colors
        glEnableVertexAttribArray(1);

#ifdef DREAM_LOG
        checkGLError();
#endif
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(GLWidgetVertex)),
            (GLvoid*)(sizeof(float)*3)
        );

#ifdef DREAM_LOG
        checkGLError();
#endif


    }

    void GLWidget::setPosition(vec3 pos)
    {
        mModelMatrix = glm::translate(mat4(1.0f),pos);
    }

    vec3 GLWidget::getPosition()
    {
       return vec3(mModelMatrix[3]);
    }

    void
    GLWidget::setViewMatrix
    (mat4 view)
    {
        mViewMatrix = view;
    }

    void GLWidget::draw()
    {

#ifdef DREAM_LOG
        checkGLError();
#endif
        if (mState->project)
        {
            auto pRuntime = mState->project->getRuntime();
            if (pRuntime)
            {
                auto sRunt = pRuntime->getActiveSceneRuntime();
                if (sRunt)
                {
                    auto cam = sRunt->getCamera();
                    if (cam)
                    {
                        mProjectionMatrix = cam->getProjectionMatrix();
                        mViewMatrix = cam->getViewMatrix();
                    }
                }

            }
        }

#ifdef DREAM_LOG
        auto log = getLog();
#endif
        if (!mVertexBuffer.empty())
        {
            glEnable(GL_LINE_SMOOTH);

#ifdef DREAM_LOG
            checkGLError();
#endif

#ifndef __APPLE__
            glLineWidth(3.0f);
            checkGLError();
#endif
            glEnable(GL_DEPTH_TEST);
            // Enable shader program
            glUseProgram(mShaderProgram);
            ShaderRuntime::CurrentShaderProgram = mShaderProgram;

#ifdef DREAM_LOG
            checkGLError();
#endif

            // Vertex Array
            glBindVertexArray(mVao);
            ShaderRuntime::CurrentVAO = mVao;

#ifdef DREAM_LOG
            checkGLError();
#endif

            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            ShaderRuntime::CurrentVBO = mVbo;

#ifdef DREAM_LOG
            checkGLError();
#endif

            // Set the projection matrix
            if (mModelUniform == -1)
            {

#ifdef DREAM_LOG
                log->error("Unable to find Uniform Location for model");
#endif
                return;
            }
            else
            {
                glUniformMatrix4fv(mModelUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));

#ifdef DREAM_LOG
                checkGLError();
#endif
            }

            // Set the projection matrix
            if (mProjectionUniform == -1)
            {

#ifdef DREAM_LOG
                log->error("Unable to find Uniform Location for projection");
#endif
                return;
            }
            else
            {
                glUniformMatrix4fv(mProjectionUniform, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));

#ifdef DREAM_LOG
                checkGLError();
#endif
            }

            // Set the view matrix
            if (mViewUniform == -1)
            {

#ifdef DREAM_LOG
                log->error("Unable to find Uniform Location for view");
#endif
                return;
            }
            else
            {
                glUniformMatrix4fv(mViewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));

#ifdef DREAM_LOG
                checkGLError();
#endif
            }

            // Draw
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));

#ifdef DREAM_LOG
            checkGLError();
#endif

            // Revert State
            glDisable(GL_LINE_SMOOTH);

#ifdef DREAM_LOG
            checkGLError();
#endif
#ifndef __APPLE__
            glLineWidth(1.0f);
            checkGLError();
#endif
        }
    }

    void
    GLWidget::setShader
    ()
    {
        mVertexShaderSource =
            "#version 330 core\n"
            "\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec3 color;\n"
            "\n"
            "out vec3 Color;\n"
            "\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "\n"
            "void main () {\n"
            "    gl_Position = projection * view * model * vec4(position,1.0) ;\n"
            "    Color = color;\n"
            "}";

        mFragmentShaderSource =
            "#version 330 core\n"
            "\n"
            "in vec3  Color;\n"
            "\n"
            "out vec4 fragColor;\n"
            "\n"
            "void main() { \n"
            "    fragColor = vec4(Color,1.0);\n"
            "}";
    }

    void
    GLWidget::initShader()
    {

#ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Initialising Shader");
#endif

        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;

        // Compile shaders
        GLint success;
        GLchar infoLog[512];

        // Vertex Shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vSource = mVertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vSource, nullptr);
        glCompileShader(vertexShader);

        // Print compile errors if any
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

#ifdef DREAM_LOG
            log->error("SHADER::VERTEX: COMPILATION_FAILED {}", infoLog);
#endif
        }

        // Fragment Shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fSource = mFragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fSource, nullptr);
        glCompileShader(fragmentShader);

        // Print compile errors if any
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

#ifdef DREAM_LOG
            log->error("SHADER::FRAGMENT: COMPILATION_FAILED {}", infoLog);
#endif
        }

        // Shader Program
        mShaderProgram = glCreateProgram();
        glAttachShader(mShaderProgram, vertexShader);
        glAttachShader(mShaderProgram, fragmentShader);
        glLinkProgram(mShaderProgram);

        // Print linking errors if any
        glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);

#ifdef DREAM_LOG
            log->error(" SHADER::PROGRAM: LINKING_FAILED {}", infoLog);
#endif
        }

        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Get Uniform Locations
        mModelUniform = glGetUniformLocation(mShaderProgram, "model");
        mViewUniform = glGetUniformLocation(mShaderProgram,"view");
        mProjectionUniform = glGetUniformLocation(mShaderProgram, "projection");
    }

    void
    GLWidget::setProjectionMatrix
    (mat4 projection)
    {
        mProjectionMatrix = projection;
    }

    void GLWidget::addVertex(GLWidgetVertex lv)
    {
        mVertexBuffer.push_back(lv);
    }
}
