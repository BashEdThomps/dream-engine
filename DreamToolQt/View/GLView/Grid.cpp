/*
 * Grid.cpp
 *
 * Created: 07 2017 by Ashley
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
#include "Grid.h"
#include <DreamCore.h>

using namespace Dream;

Grid::Grid
(
    QObject* parent,
    float majorSpacing, float minorSpacing, float size,
    vec3 majorColour, vec3 minorColour
)
    : QObject(parent),

      mSize(size),
      mMajorSpacing(majorSpacing),
      mMajorColour(majorColour),
      mMinorSpacing(minorSpacing),
      mMinorColour(minorColour),
      mInitialised(false),
      mShaderProgram(0)
{
}

Grid::~Grid
()
{
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
Grid::isInitialised
()
{
    return mInitialised;
}

void
Grid::init
()
{
    initShader();
    initVaoVbo();
    initMajorGridData();
    initMinorGridData();
    mInitialised = true;
}

void
Grid::draw
()
{
    if (!mVertexBuffer.empty())
    {
        glEnable(GL_LINE_SMOOTH);
        if (Constants::DEBUG)
        {
            cout << "Grid: Drawing all - " << mVertexBuffer.size()/2 << " lines." << endl;
        }

        // Enable shader program
        glUseProgram(mShaderProgram);

        // Set the projection matrix
        GLint projUniform = glGetUniformLocation(mShaderProgram, "projection");
        if (projUniform == -1)
        {
            cerr << "Grid: Unable to find Uniform Location for projection" << endl;
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
            cerr << "Grid: Unable to find Uniform Location for view" << endl;
            return;
        }
        else
        {
            glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
        }

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(GridVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);

        // Vertex Array
        glBindVertexArray(mVao);

        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                    0, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(GridVertex)),
                    static_cast<GLvoid*>(0)
                    );
        // Vertex Colors
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
                    1, 3, GL_FLOAT, GL_FALSE,
                    static_cast<GLint>(sizeof(GridVertex)),
                    (GLvoid*)offsetof(GridVertex, Color)
                    );
        // Draw
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertexBuffer.size()));

        // Revert State
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
        glDisable(GL_LINE_SMOOTH);

        // Clear old buffer
        mVertexBuffer.clear();
    }
}

void
Grid::initShader
()
{
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
        cerr << "Grid: SHADER:VERTEX:COMPILATION_FAILED\n" << infoLog << endl;
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
        cerr << "Grid: SHADER:FRAGMENT:COMPILATION_FAILED\n" << infoLog << endl;
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
        cerr << "Grid: SHADER:PROGRAM:LINKING_FAILED\n" << infoLog << endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void
Grid::initVaoVbo
()
{
    glGenVertexArrays(1,&mVao);
    glGenBuffers(1,&mVbo);
}

void
Grid::initMajorGridData
()
{
    float halfSize = (mSize/2.0f);

    // Major Grid
    for (float y = -halfSize; y <= halfSize; y += mMajorSpacing)
    {
        GridVertex majorStart, majorEnd;

        majorStart.Position = vec3(-halfSize,y,0.0f);
        majorStart.Color = mMajorColour;

        majorEnd.Position = vec3(halfSize,y,0.0f);
        majorEnd.Color = mMajorColour;

        mVertexBuffer.push_back(majorStart);
        mVertexBuffer.push_back(majorEnd);
    }

    for (float x = -halfSize; x <= halfSize; x += mMajorSpacing)
    {
        GridVertex majorStart, majorEnd;

        majorStart.Position = vec3(x,-halfSize,0.0f);
        majorStart.Color = mMajorColour;

        majorEnd.Position = vec3(x,halfSize,0.0f);
        majorEnd.Color = mMajorColour;

        mVertexBuffer.push_back(majorStart);
        mVertexBuffer.push_back(majorEnd);
    }
}

void
Grid::initMinorGridData
()
{
    float halfSize = (mSize/2.0f);

    // Minor Grid
    for (float y = -halfSize; y <= halfSize; y += mMinorSpacing)
    {
        GridVertex minorStart, minorEnd;

        minorStart.Position = vec3(-halfSize,y,0.0f);
        minorStart.Color = mMinorColour;

        minorEnd.Position = vec3(halfSize,y,0.0f);
        minorEnd.Color = mMinorColour;

        mVertexBuffer.push_back(minorStart);
        mVertexBuffer.push_back(minorEnd);
    }

    for (float x = -halfSize; x <= halfSize; x += mMinorSpacing)
    {
        GridVertex minorStart, minorEnd;

        minorStart.Position = vec3(x,-halfSize,0.0f);
        minorStart.Color = mMinorColour;

        minorEnd.Position = vec3(x,halfSize,0.0f);
        minorEnd.Color = mMinorColour;

        mVertexBuffer.push_back(minorStart);
        mVertexBuffer.push_back(minorEnd);
    }
}

void
Grid::setViewMatrix
(mat4 view)
{
    mViewMatrix = view;
}


void
Grid::setProjectionMatrix
(mat4 projection)
{
    mProjectionMatrix = projection;
}
