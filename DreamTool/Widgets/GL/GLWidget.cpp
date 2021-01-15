#include "GLWidget.h"
#include "DTContext.h"
#include <DreamCore.h>
#include <glm/gtc/matrix_transform.hpp>

namespace DreamTool
{
    GLWidget::GLWidget
    (DTContext* project, bool visible)
        : DTWidget (project, visible),
          mModelMatrix(mat4(1.0f)),
          mViewMatrix(mat4(1.0f)),
          mProjectionMatrix(mat4(1.0f)),
          mVao(0),
          mVbo(0),
          mShaderProgram(0)
    {
    }

    void GLWidget::init()
    {
        setShader();
        initShader();
        initVaoVbo();
    }

    GLWidget::~GLWidget()
    {

        LOG_DEBUG("GLWidget: Destroying");
        if (mVao > 0)
        {
            glDeleteVertexArrays(1,&mVao);

            GLCheckError();
        }
        if (mVbo > 0)
        {
            glDeleteBuffers(1,&mVbo);

            GLCheckError();
        }
        if (mShaderProgram > 0)
        {
            glDeleteProgram(mShaderProgram);

            GLCheckError();
        }
    }

    void
    GLWidget::initVaoVbo
    ()
    {
        glGenVertexArrays(1,&mVao);
        GLCheckError();

        glBindVertexArray(mVao);
        ShaderRuntime::CurrentVAO = mVao;
        GLCheckError();

        glGenBuffers(1,&mVbo);
        GLCheckError();

        glBindBuffer(GL_ARRAY_BUFFER,mVbo);
        ShaderRuntime::CurrentVBO = mVbo;

        // Vertex Positions
        glEnableVertexAttribArray(0);
        GLCheckError();
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(GLWidgetVertex)),
            static_cast<GLvoid*>(0)
        );

        GLCheckError();

        // Vertex Colors
        glEnableVertexAttribArray(1);

        GLCheckError();
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(GLWidgetVertex)),
            (GLvoid*)(sizeof(float)*3)
        );

        GLCheckError();
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

        GLCheckError();
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
                    else
                    {
                        return;
                    }
                }
                else
                {
                    return;
                }

            }
            else
            {
                return;
            }
        }

        if (!mVertexBuffer.empty())
        {
            glEnable(GL_LINE_SMOOTH);

            GLCheckError();

            glEnable(GL_DEPTH_TEST);
            // Enable shader program
            glUseProgram(mShaderProgram);
            ShaderRuntime::CurrentShaderProgram = mShaderProgram;

            GLCheckError();

            // Vertex Array
            glBindVertexArray(mVao);
            ShaderRuntime::CurrentVAO = mVao;

            GLCheckError();

            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            ShaderRuntime::CurrentVBO = mVbo;

            GLCheckError();

            // Set the projection matrix
            if (mModelUniform == -1)
            {
                LOG_ERROR("GLWidget: Unable to find Uniform Location for model");
                return;
            }
            else
            {
                glUniformMatrix4fv(mModelUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
                GLCheckError();
            }

            // Set the projection matrix
            if (mProjectionUniform == -1)
            {
                LOG_ERROR("GLWidget: Unable to find Uniform Location for projection");
                return;
            }
            else
            {
                glUniformMatrix4fv(mProjectionUniform, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
                GLCheckError();
            }

            // Set the view matrix
            if (mViewUniform == -1)
            {
                LOG_ERROR("GLWidget: Unable to find Uniform Location for view");
                return;
            }
            else
            {
                glUniformMatrix4fv(mViewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
                GLCheckError();
            }

            // Draw
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));
            GLCheckError();

            // Revert State
            glDisable(GL_LINE_SMOOTH);
            GLCheckError();
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
        LOG_DEBUG("GLWidget: Initialising Shader");

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
            LOG_ERROR("GLWidget: SHADER::VERTEX: COMPILATION_FAILED {}", infoLog);
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
            LOG_ERROR("GLWidget: SHADER::FRAGMENT: COMPILATION_FAILED {}", infoLog);
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
            LOG_ERROR("GLWidget:  SHADER::PROGRAM: LINKING_FAILED {}", infoLog);
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
