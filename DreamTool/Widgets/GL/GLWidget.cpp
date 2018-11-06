//
//  GLWidget.cpp
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#include "GLWidget.h"

namespace DreamTool
{
    GLWidget::GLWidget
    (Project* project)
        : DTWidget (project),
          mModelMatrix(mat4(1.0f)),
          mViewMatrix(mat4(1.0f)),
          mProjectionMatrix(mat4(1.0f)),
          mVao(0),
          mVbo(0),
          mShaderProgram(0)
    {
        setLogClassName("GLWidget");
    }

    void GLWidget::init()
    {
        setShader();
        initShader();
        initVaoVbo();
    }

    GLWidget::~GLWidget()
    {
        auto log=getLog();
        log->debug("Destroying");
        if (mVao > 0)
        {
            glDeleteVertexArrays(1,&mVao);
            checkGLError();
        }
        if (mVbo > 0)
        {
            glDeleteBuffers(1,&mVbo);
            checkGLError();
        }
        if (mShaderProgram > 0)
        {
            glDeleteProgram(mShaderProgram);
            checkGLError();
        }
    }

    void
    GLWidget::initVaoVbo
    ()
    {
        glGenVertexArrays(1,&mVao);
        checkGLError();
        glBindVertexArray(mVao);
        ShaderInstance::CurrentVAO = mVao;
        checkGLError();

        glGenBuffers(1,&mVbo);
        checkGLError();

        glBindBuffer(GL_ARRAY_BUFFER,mVbo);
        ShaderInstance::CurrentVBO = mVbo;

        // Vertex Positions
        glEnableVertexAttribArray(0);
        checkGLError();
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(LineVertex)),
            static_cast<GLvoid*>(0)
        );
        checkGLError();

        // Vertex Colors
        glEnableVertexAttribArray(1);
        checkGLError();
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(LineVertex)),
            (GLvoid*)(sizeof(float)*3)
        );
        checkGLError();


    }

    void GLWidget::setPosition(vec3 pos)
    {
       mModelMatrix = glm::translate(mat4(1.0f),pos);
    }

    void
    GLWidget::setViewMatrix
    (mat4 view)
    {
        mViewMatrix = view;
    }

    void GLWidget::draw()
    {
		checkGLError();
        if (mProject)
        {
            auto pRuntime = mProject->getProjectRuntime();
            if (pRuntime)
            {
                auto gfx = pRuntime->getGraphicsComponent();
                if (gfx)
                {
                    mViewMatrix = gfx->getViewMatrix();
                    mProjectionMatrix = gfx->getProjectionMatrix();
                }
            }
        }
        auto log = getLog();
        if (!mVertexBuffer.empty())
        {
#ifndef __APPLE__
            glEnable(GL_LINE_SMOOTH);
            checkGLError();
            glLineWidth(3.0f);
            checkGLError();
#endif
            // Enable shader program
            glUseProgram(mShaderProgram);
            ShaderInstance::CurrentShaderProgram = mShaderProgram;
            checkGLError();

            // Vertex Array
            glBindVertexArray(mVao);
            ShaderInstance::CurrentVAO = mVao;
            checkGLError();

            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            ShaderInstance::CurrentVBO = mVbo;
            checkGLError();

            // Set the projection matrix
            GLint modelUniform = glGetUniformLocation(mShaderProgram, "model");
            if (modelUniform == -1)
            {
                log->error("Unable to find Uniform Location for model");
                return;
            }
            else
            {
                glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
            checkGLError();
            }

            // Set the projection matrix
            GLint projUniform = glGetUniformLocation(mShaderProgram, "projection");
            if (projUniform == -1)
            {
                log->error("Unable to find Uniform Location for projection");
                return;
            }
            else
            {
                glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
            checkGLError();
            }

            // Set the view matrix
            GLint viewUniform = glGetUniformLocation(mShaderProgram, "view");
            if (viewUniform == -1)
            {
                log->error("Unable to find Uniform Location for view");
                return;
            }
            else
            {
                glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
            checkGLError();
            }

            glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(LineVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
            checkGLError();

            // Draw
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));
            checkGLError();

            // Revert State
#ifndef __APPLE__
            glDisable(GL_LINE_SMOOTH);
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
        auto log = getLog();
        log->debug("Initialising Shader");

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
            log->error("SHADER::VERTEX: COMPILATION_FAILED {}", infoLog);
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
            log->error("SHADER::FRAGMENT: COMPILATION_FAILED {}", infoLog);
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
            log->error(" SHADER::PROGRAM: LINKING_FAILED {}", infoLog);
        }

        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void
    GLWidget::setProjectionMatrix
    (mat4 projection)
    {
        mProjectionMatrix = projection;
    }

    void GLWidget::addLineVertex(LineVertex lv)
    {
        mVertexBuffer.push_back(lv);
    }
}
