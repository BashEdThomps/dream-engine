/*
 * GLDrawable.cpp
 *
 * Created: 09 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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
#include "GLDrawable.h"
#include <DreamCore.h>
#include <QDebug>
#include "spdlog/spdlog.h"

using namespace std;
using namespace glm;
using namespace Dream;

GLDrawable::GLDrawable
(QObject *parent)
    : QObject(parent),
      mVao(0),
      mVbo(0),
      mShaderProgram(0),
      mInitialised(false)
{
    auto log = spdlog::get("GLDrawable");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("GLDrawable");
    }
    log->info("Constructing");
}

GLDrawable::~GLDrawable
()
{
    auto log = spdlog::get("GLDrawable");
    log->info("Destroying");
    if (mVao > 0)
    {
        glDeleteVertexArrays(1,&mVao);
    }
    if (mVbo > 0)
    {
        glDeleteBuffers(1,&mVbo);
    }

    if (mShaderProgram > 0)
    {
        glDeleteProgram(mShaderProgram);
    }
}

bool
GLDrawable::isInitialised
()
{
    return mInitialised;
}

void
GLDrawable::initGLDrawable
()
{
    auto log = spdlog::get("GLDrawable");
    log->info("Initialising");
    initShader();
    initVaoVbo();
    mInitialised = true;
}

void
GLDrawable::preRender
()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    checkGLError();
}

void
GLDrawable::postRender
()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable (GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    checkGLError();
}

void
GLDrawable::draw
()
{
    auto log = spdlog::get("GLDrawable");
    if (!mVertexBuffer.empty())
    {
        preRender();
            log->info("Drawing {} lines", mVertexBuffer.size()/2);

        // Enable shader program
        glUseProgram(mShaderProgram);

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
        }

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(LineVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);

        // Vertex Array
        glBindVertexArray(mVao);

        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                    0, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(LineVertex)),
                    static_cast<GLvoid*>(0)
                    );
        // Vertex Colors
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                    1, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(LineVertex)),
                    (GLvoid*)offsetof(LineVertex, Color)
                    );
        // Draw
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));

        // Revert State
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
        postRender();
    }
}

void
GLDrawable::initShader
()
{
    auto log = spdlog::get("GLDrawable");
    log->info("Initialising Shader");

    string vertexShaderSource;
    string fragmentShaderSource;
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

    vertexShaderSource =
        "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "\n"
        "out vec3 Color;\n"
        "\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main () {\n"
        "    gl_Position = projection * view * vec4(position,1.0);\n"
        "    Color = color;\n"
        "}";

    fragmentShaderSource =
        "#version 330 core\n"
        "\n"
        "in vec3  Color;\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main() { \n"
        "    fragColor = vec4(Color,1.0);\n"
        "}";

    // Compile shaders
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vSource = vertexShaderSource.c_str();
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
    const char *fSource = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fSource, nullptr);
    glCompileShader(fragmentShader);

    // Print compile errors if any
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        log->error("SHADER::FRAGMENT: COMPILATION_FAILED {}",  infoLog);
    }

    // Shader Program
    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);
    glLinkProgram(mShaderProgram);

    // Print linking errors if any
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
        log->error(" SHADER::PROGRAM: LINKING_FAILED {}"  ,infoLog );
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(0);
}

void
GLDrawable::initVaoVbo
()
{
    glGenVertexArrays(1,&mVao);
    glGenBuffers(1,&mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void
GLDrawable::setViewMatrix
(mat4 view)
{
    mViewMatrix = view;
}


void
GLDrawable::setProjectionMatrix
(mat4 projection)
{
    mProjectionMatrix = projection;
}
