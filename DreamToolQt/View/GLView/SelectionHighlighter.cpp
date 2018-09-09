/*
 * SelectionHighlighter.cpp
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

#include "SelectionHighlighter.h"
#include <spdlog/spdlog.h>

using Dream::AssimpModelInstance;

using glm::quat;
using glm::mat4;
using Dream::Transform3D;

SelectionHighlighter::SelectionHighlighter
(QObject *parent)
    : GLDrawable(parent),
      mSelectedObjectHandle(nullptr),
      mSelectionColour(vec3(1.0f, 1.0f, 0.0f)),
      mXColour(vec3(1.0f, 0.0f, 0.0f)),
      mYColour(vec3(0.0f, 1.0f, 0.0f)),
      mZColour(vec3(0.0f, 0.0f, 1.0f)),
      mOffset(0.1f)
{
    auto log = spdlog::get("SelectionHighlighter");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("SelectionHighlighter");
    }
    log->info("Constructing Object");
}

SelectionHighlighter::~SelectionHighlighter
()
{
    auto log = spdlog::get("SelectionHighlighter");
    log->info("Destructing Object");
}

void
SelectionHighlighter::init
()
{
    auto log = spdlog::get("SelectionHighlighter");
    log->info("Initialising");
    initGLDrawable();
}

void
SelectionHighlighter::setSelectedSceneObjectRuntimeHandle
(shared_ptr<SceneObjectRuntime> selected)
{
    auto log = spdlog::get("SelectionHighlighter");
    if (selected != nullptr)
    {
        log->info("Selecting {}",selected->getNameAndUuidString());
    }
    mSelectedObjectHandle = selected;
    updateVertexBuffer();
}

void
SelectionHighlighter::updateVertexBuffer
()
{
    auto log = spdlog::get("SelectionHighlighter");
    log->info("Updating Vertex Buffer") ;
    mVertexBuffer.clear();

    if (!mSelectedObjectHandle)
    {
        log->info("No object selected");
        return;
    }

    BoundingBox bounds;

    if (mSelectedObjectHandle->hasModelInstance())
    {
        bounds = mSelectedObjectHandle->getModelInstance()->getBoundingBox();
    }

    log->info("Minimum Bounds {},{},{}",bounds.minimum.x ,bounds.minimum.y, bounds.minimum.z);
    log->info("Maximum Bounds {},{},{}",bounds.maximum.x ,bounds.maximum.y, bounds.maximum.z);


    // Top Quad

    LineVertex topFront1, topFront2;
    topFront1.Position = vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    topFront1.Color = mSelectionColour;
    mVertexBuffer.push_back(topFront1);
    topFront2.Position = vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    topFront2.Color = mSelectionColour;
    mVertexBuffer.push_back(topFront2);

    LineVertex topBack1, topBack2;
    topBack1.Position = vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    topBack1.Color = mSelectionColour;
    mVertexBuffer.push_back(topBack1);
    topBack2.Position = vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    topBack2.Color = mSelectionColour;
    mVertexBuffer.push_back(topBack2);

    LineVertex topLeft1, topLeft2;
    topLeft1.Position = vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    topLeft1.Color = mSelectionColour;
    mVertexBuffer.push_back(topLeft1);
    topLeft2.Position = vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    topLeft2.Color = mSelectionColour;
    mVertexBuffer.push_back(topLeft2);

    LineVertex topRight1, topRight2;
    topRight1.Position = vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    topRight1.Color = mSelectionColour;
    mVertexBuffer.push_back(topRight1);
    topRight2.Position = vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    topRight2.Color = mSelectionColour;
    mVertexBuffer.push_back(topRight2);

    // Bottom Quad

    LineVertex bottomFront1, bottomFront2;
    bottomFront1.Position = vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    bottomFront1.Color = mSelectionColour;
    mVertexBuffer.push_back(bottomFront1);
    bottomFront2.Position = vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    bottomFront2.Color = mSelectionColour;
    mVertexBuffer.push_back(bottomFront2);

    LineVertex bottomBack1, bottomBack2;
    bottomBack1.Position = vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    bottomBack1.Color = mSelectionColour;
    mVertexBuffer.push_back(bottomBack1);
    bottomBack2.Position = vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    bottomBack2.Color = mSelectionColour;
    mVertexBuffer.push_back(bottomBack2);

    LineVertex bottomLeft1, bottomLeft2;
    bottomLeft1.Position = vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    bottomLeft1.Color = mSelectionColour;
    mVertexBuffer.push_back(bottomLeft1);
    bottomLeft2.Position = vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    bottomLeft2.Color = mSelectionColour;
    mVertexBuffer.push_back(bottomLeft2);

    LineVertex bottomRight1, bottomRight2;
    bottomRight1.Position = vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    bottomRight1.Color = mSelectionColour;
    mVertexBuffer.push_back(bottomRight1);
    bottomRight2.Position = vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    bottomRight2.Color = mSelectionColour;
    mVertexBuffer.push_back(bottomRight2);

    // Verticals

    LineVertex frontLeft1, frontLeft2;
    frontLeft1.Position = vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    frontLeft1.Color = mSelectionColour;
    mVertexBuffer.push_back(frontLeft1);
    frontLeft2.Position = vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    frontLeft2.Color = mSelectionColour;
    mVertexBuffer.push_back(frontLeft2);

    LineVertex frontRight1, frontRight2;
    frontRight1.Position = vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    frontRight1.Color = mSelectionColour;
    mVertexBuffer.push_back(frontRight1);
    frontRight2.Position = vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    frontRight2.Color = mSelectionColour;
    mVertexBuffer.push_back(frontRight2);

    LineVertex backLeft1, backLeft2;
    backLeft1.Position = vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    backLeft1.Color = mSelectionColour;
    mVertexBuffer.push_back(backLeft1);
    backLeft2.Position = vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    backLeft2.Color = mSelectionColour;
    mVertexBuffer.push_back(backLeft2);

    LineVertex backRight1, backRight2;
    backRight1.Position = vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    backRight1.Color = mSelectionColour;
    mVertexBuffer.push_back(backRight1);
    backRight2.Position = vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    backRight2.Color = mSelectionColour;
    mVertexBuffer.push_back(backRight2);

    // Normal Lines

    LineVertex x1, x2;
    x1.Position = vec3(0);
    x1.Color = mXColour;
    mVertexBuffer.push_back(x1);
    x2.Position = vec3(fabs(bounds.maxDimension*2),0,0);
    x2.Color = mXColour;
    mVertexBuffer.push_back(x2);

    LineVertex y1, y2;
    y1.Position = vec3(0);
    y1.Color = mYColour;
    mVertexBuffer.push_back(y1);
    y2.Position = vec3(0,fabs(bounds.maxDimension*2),0);
    y2.Color = mYColour;
    mVertexBuffer.push_back(y2);

    LineVertex z1, z2;
    z1.Position = vec3(0);
    z1.Color = mZColour;
    mVertexBuffer.push_back(z1);
    z2.Position = vec3(0,0,fabs(bounds.maxDimension*2));
    z2.Color = mZColour;
    mVertexBuffer.push_back(z2);
}

void
SelectionHighlighter::draw
()
{
    auto log = spdlog::get("SelectionHighlighter");
    if (!mVertexBuffer.empty())
    {
        preRender();
        glLineWidth(4);
        log->info("Drawing all - {} lines", mVertexBuffer.size()/2);

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
            glUniformMatrix4fv(projUniform, 1, GL_FALSE, value_ptr(mProjectionMatrix));
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
            glUniformMatrix4fv(viewUniform, 1, GL_FALSE, value_ptr(mViewMatrix));
        }

        // Set the model matrix
        if (mSelectedObjectHandle)
        {
            GLint modelUniform = glGetUniformLocation(mShaderProgram, "model");
            if (modelUniform == -1)
            {
                log->error("Unable to find Uniform Location for Model");
                return;
            }
            else
            {
                mat4 modelMatrix(1.0f);
                if (mSelectedObjectHandle->hasModelInstance())
                {
                   modelMatrix = mSelectedObjectHandle->getModelInstance()->getModelMatrix();
                }
                else
                {
                    // Get raw data
                    shared_ptr<Transform3D> transform = mSelectedObjectHandle->getTransform();
                    vec3 translationValue = transform->getTranslation();
                    quat rotValue = transform->getOrientation();
                    vec3 scaleValue = transform->getScale();
                    // Translate
                    modelMatrix = translate(modelMatrix,translationValue);
                    // Rotate
                    mat4 rotMat = mat4_cast(rotValue);
                    modelMatrix = modelMatrix * rotMat;
                    // Scale
                    modelMatrix = scale(modelMatrix, scaleValue);
                }
                glUniformMatrix4fv(modelUniform, 1, GL_FALSE, value_ptr(modelMatrix));
            }
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
        glLineWidth(1);
    }
}

void
SelectionHighlighter::initShader
()
{
    auto log = spdlog::get("SelectionHighlighter");
    log->info("Initialising shader");

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
            "uniform mat4 model;\n"
            "\n"
            "void main () {\n"
            "    gl_Position = projection * view * model * vec4(position,1.0);\n"
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
        log->error("SHADER:VERTEX:COMPILATION_FAILED\n{}", infoLog);
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
        log->error("SHADER:FRAGMENT:COMPILATION_FAILED\n{}", infoLog);
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
        log->error("SHADER:PROGRAM:LINKING_FAILED\n{}" ,infoLog);
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

shared_ptr<SceneObjectRuntime>
SelectionHighlighter::getSelectedObject
()
{
    return mSelectedObjectHandle;
}
