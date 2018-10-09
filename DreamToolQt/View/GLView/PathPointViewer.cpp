/*
 * PathPointViewer.cpp
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

#include "PathPointViewer.h"
#include "DreamCore.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using Dream::ModelInstance;
using Dream::Constants;
using Dream::Transform3D;
using Dream::ShaderInstance;

using glm::quat;
using glm::mat4;
using glm::vec3;

PathPointViewer::PathPointViewer
(QObject *parent)
    : GLDrawable(parent),
      mPathInstance(nullptr),
      mSelectedColour(vec3(0.0f, 1.0f, 0.0f)),
      mUnselectedColour(vec3(0.75f, 0.75f, 0.0f)),
      mCurveColour(vec3(1.0f, 0.0f, 1.0f)),
      mTangentColour(vec3(0.25f,1.0f,0.0f)),
      mDrawTangent(false),
      mNodeSize(0.25f),
      mSelectedCp(-1),
      mUStep(0.05),
      mVisible(true)

{
    auto log = spdlog::get("PathPointViewer");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("PathPointViewer");
    }
    log->debug("Constructing Object");
}

PathPointViewer::~PathPointViewer
()
{
    auto log = spdlog::get("PathPointViewer");
    log->debug("Destructing Object");
}

void
PathPointViewer::init
()
{
    auto log = spdlog::get("PathPointViewer");
    log->debug("Initialising");
    initGLDrawable();
}

void
PathPointViewer::setPathDefinition
(PathDefinition* selected)
{
    auto log = spdlog::get("PathPointViewer");
    if (selected != nullptr)
    {
        log->debug("Selecting {}",selected->getNameAndUuidString());
    }
    else
    {
        log->debug("Path selection cleared");
    }
    mPathDefinition = selected;
    updateVertexBuffer();
}

void
PathPointViewer::updateVertexBuffer
()
{
    auto log = spdlog::get("PathPointViewer");
    log->debug("Updating Vertex Buffer") ;
    mVertexBuffer.clear();

    if (mPathDefinition == nullptr)
    {
        log->debug("No object selected");
        return;
    }

    json* controlPoints = mPathDefinition->getControlPoints();
    if (controlPoints->is_array())
    {
        for (json cp : *controlPoints)
        {
            generateNode(cp);
        }
    }
    if (controlPoints->size() > 3)
    {
        generateSpline();
    }
}

bool
PathPointViewer::getDrawTangent
()
{
    return mDrawTangent;
}

void
PathPointViewer::setDrawTangent
(bool drawTangent)
{
    mDrawTangent = drawTangent;
}

void
PathPointViewer::clearRuntime
()
{
    mPathDefinition = nullptr;
}

size_t
PathPointViewer::getTangentIndex
()
{
    return mTangentIndex;
}

void
PathPointViewer::setTangentIndex
(size_t& tangentIndex)
{
    mTangentIndex = tangentIndex;
}

void
PathPointViewer::generateSpline
()
{
    if (mPathInstance == nullptr)
    {
        mPathInstance = unique_ptr<PathInstance>(new PathInstance(mPathDefinition,nullptr));
   }
   mPathInstance->load("");

    emit notifyNumberOfTangentsChanged(
        static_cast<int>(mPathInstance->getSplineDerivatives().size())
    );

   vector<vec3> splinePoints = mPathInstance->getSplinePoints();

   for (size_t i=1; i < splinePoints.size(); i++)
   {
       vec3 current = splinePoints.at(i-1);
       vec3 next = splinePoints.at(i);

       LineVertex v1,v2;
       v1.Position  = current;
       v1.Color = mCurveColour;
       v2.Position = next;
       v2.Color = mCurveColour;
       mVertexBuffer.push_back(v1);
       mVertexBuffer.push_back(v2);
   }

   auto tans = mPathInstance->getSplineDerivatives();

   if (mDrawTangent && mTangentIndex > 0 && mTangentIndex < tans.size())
   {
       pair<vec3,vec3> tangent = tans.at(mTangentIndex);
       LineVertex v1, v2;
       v1.Position = tangent.first;
       v1.Color = vec3(1,0,0);
       v2.Position = tangent.second;
       v2.Color = vec3(0,1,0);
       mVertexBuffer.push_back(v1);
       mVertexBuffer.push_back(v2);
   }
}

void
PathPointViewer::generateNode
(json cp)
{
    auto log = spdlog::get("PathPointViewer");
    float x,y,z;
    int index = cp[Constants::ASSET_ATTR_INDEX];
    x = cp[Constants::X];
    y = cp[Constants::Y];
    z = cp[Constants::Z];

    log->trace("Generating node cube for {} at ({},{},{})",index,x,y,z);

    vec3 colour = (mSelectedCp ==  index ? mSelectedColour : mUnselectedColour);
    log->trace("Selected? {}",mSelectedCp == index);

    // Top Quad

    LineVertex topFront1, topFront2;
    topFront1.Position = vec3(x-mNodeSize, y+mNodeSize, z-mNodeSize);
    topFront1.Color = colour;
    mVertexBuffer.push_back(topFront1);
    topFront2.Position = vec3(x+mNodeSize, y+mNodeSize, z-mNodeSize);
    topFront2.Color = colour;
    mVertexBuffer.push_back(topFront2);

    LineVertex topBack1, topBack2;
    topBack1.Position = vec3(x-mNodeSize, y+mNodeSize, z+mNodeSize);
    topBack1.Color = colour;
    mVertexBuffer.push_back(topBack1);
    topBack2.Position = vec3(x+mNodeSize, y+mNodeSize, z+mNodeSize);
    topBack2.Color = colour;
    mVertexBuffer.push_back(topBack2);

    LineVertex topLeft1, topLeft2;
    topLeft1.Position = vec3(x-mNodeSize, y+mNodeSize, z-mNodeSize);
    topLeft1.Color = colour;
    mVertexBuffer.push_back(topLeft1);
    topLeft2.Position = vec3(x-mNodeSize, y+mNodeSize, z+mNodeSize);
    topLeft2.Color = colour;
    mVertexBuffer.push_back(topLeft2);

    LineVertex topRight1, topRight2;
    topRight1.Position = vec3(x+mNodeSize, y+mNodeSize, z-mNodeSize);
    topRight1.Color = colour;
    mVertexBuffer.push_back(topRight1);
    topRight2.Position = vec3(x+mNodeSize, y+mNodeSize, z+mNodeSize);
    topRight2.Color = colour;
    mVertexBuffer.push_back(topRight2);

    // Bottom Quad

    LineVertex bottomFront1, bottomFront2;
    bottomFront1.Position = vec3(x-mNodeSize, y-mNodeSize, z-mNodeSize);
    bottomFront1.Color = colour;
    mVertexBuffer.push_back(bottomFront1);
    bottomFront2.Position = vec3(x+mNodeSize, y-mNodeSize, z-mNodeSize);
    bottomFront2.Color = colour;
    mVertexBuffer.push_back(bottomFront2);

    LineVertex bottomBack1, bottomBack2;
    bottomBack1.Position = vec3(x-mNodeSize, y-mNodeSize, z+mNodeSize);
    bottomBack1.Color = colour;
    mVertexBuffer.push_back(bottomBack1);
    bottomBack2.Position = vec3(x+mNodeSize, y-mNodeSize, z+mNodeSize);
    bottomBack2.Color = colour;
    mVertexBuffer.push_back(bottomBack2);

    LineVertex bottomLeft1, bottomLeft2;
    bottomLeft1.Position = vec3(x-mNodeSize, y-mNodeSize, z-mNodeSize);
    bottomLeft1.Color = colour;
    mVertexBuffer.push_back(bottomLeft1);
    bottomLeft2.Position = vec3(x-mNodeSize, y-mNodeSize, z+mNodeSize);
    bottomLeft2.Color = colour;
    mVertexBuffer.push_back(bottomLeft2);

    LineVertex bottomRight1, bottomRight2;
    bottomRight1.Position = vec3(x+mNodeSize, y-mNodeSize, z-mNodeSize);
    bottomRight1.Color = colour;
    mVertexBuffer.push_back(bottomRight1);
    bottomRight2.Position = vec3(x+mNodeSize, y-mNodeSize, z+mNodeSize);
    bottomRight2.Color = colour;
    mVertexBuffer.push_back(bottomRight2);

    // Verticals

    LineVertex frontLeft1, frontLeft2;
    frontLeft1.Position = vec3(x-mNodeSize, y-mNodeSize, z-mNodeSize);
    frontLeft1.Color = colour;
    mVertexBuffer.push_back(frontLeft1);
    frontLeft2.Position = vec3(x-mNodeSize, y+mNodeSize, z-mNodeSize);
    frontLeft2.Color = colour;
    mVertexBuffer.push_back(frontLeft2);

    LineVertex frontRight1, frontRight2;
    frontRight1.Position = vec3(x+mNodeSize, y-mNodeSize, z-mNodeSize);
    frontRight1.Color = colour;
    mVertexBuffer.push_back(frontRight1);
    frontRight2.Position = vec3(x+mNodeSize, y+mNodeSize, z-mNodeSize);
    frontRight2.Color = colour;
    mVertexBuffer.push_back(frontRight2);

    LineVertex backLeft1, backLeft2;
    backLeft1.Position = vec3(x-mNodeSize, y-mNodeSize, z+mNodeSize);
    backLeft1.Color = colour;
    mVertexBuffer.push_back(backLeft1);
    backLeft2.Position = vec3(x-mNodeSize, y+mNodeSize, z+mNodeSize);
    backLeft2.Color = colour;
    mVertexBuffer.push_back(backLeft2);

    LineVertex backRight1, backRight2;
    backRight1.Position = vec3(x+mNodeSize, y-mNodeSize, z+mNodeSize);
    backRight1.Color = colour;
    mVertexBuffer.push_back(backRight1);
    backRight2.Position = vec3(x+mNodeSize, y+mNodeSize, z+mNodeSize);
    backRight2.Color = colour;
    mVertexBuffer.push_back(backRight2);
}

void
PathPointViewer::draw
()
{
    auto log = spdlog::get("PathPointViewer");
    if (!mVisible)
    {
        log->trace("Not visible");
        return;
    }

    if (!mVertexBuffer.empty())
    {
        preRender();
        log->trace("Drawing all - {} lines", mVertexBuffer.size()/2);

        // Enable shader program
        useShader();

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
        if (mPathDefinition)
        {
            GLint modelUniform = glGetUniformLocation(mShaderProgram, "model");
            if (modelUniform == -1)
            {
                log->error("Unable to find Uniform Location for Model");
                return;
            }
            else
            {
                mat4 modelMatrix(1.0);
                glUniformMatrix4fv(modelUniform, 1, GL_FALSE, value_ptr(modelMatrix));
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(LineVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
        ShaderInstance::CurrentVBO = mVbo;

        // Vertex Array
        glBindVertexArray(mVao);
        ShaderInstance::CurrentVAO = mVao;

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
        postRender();
    }
}

void
PathPointViewer::initShader
()
{
    auto log = spdlog::get("PathPointViewer");
    log->debug("Initialising shader");

    string vertexShaderSource;
    string fragmentShaderSource;
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

    vertexShaderSource =
            "#version 330 core\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec3 color;\n"
            "out vec3 Color;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "uniform mat4 model;\n"
            "void main () {\n"
            "    gl_Position = projection * view * model * vec4(position,1.0);\n"
            "    Color = color;\n"
            "}";

    fragmentShaderSource =
            "#version 330 core\n"
            "in vec3  Color;\n"
            "out vec4 fragColor;\n"
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

void
PathPointViewer::onUpdateRequested
()
{
    updateVertexBuffer();
}

void
PathPointViewer::onSelectionChanged
(int index)
{
    mSelectedCp = index;
    updateVertexBuffer();
}

void
PathPointViewer::onTangentIndexChanged
(int val)
{
    auto log = spdlog::get("PathPointViewer");
    log->trace("onTangentIndexChanged {}",val);
    mTangentIndex = static_cast<size_t>(val);
    updateVertexBuffer();
}

void
PathPointViewer::onTangentVisibilityChanged
(bool draw)
{
    auto log = spdlog::get("PathPointViewer");
    log->trace("onTangentVisibilityChanged {}",draw);
    mDrawTangent = draw;
    updateVertexBuffer();
}

void
PathPointViewer::setPathVisible
(bool visible)
{
    mVisible = visible;
}

bool
PathPointViewer::getPathVisible
()
const
{
   return mVisible;
}
