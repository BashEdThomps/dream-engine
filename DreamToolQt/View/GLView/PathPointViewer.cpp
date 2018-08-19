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

using Dream::AssimpModelInstance;
using Dream::Constants;
using Dream::Transform3D;

using glm::quat;
using glm::mat4;
using glm::vec3;

PathPointViewer::PathPointViewer
(QObject *parent)
    : GLDrawable(parent),
      mPathDefinitionHandle(nullptr),
      mSelectedColour(vec3(0.0f, 1.0f, 0.0f)),
      mUnselectedColour(vec3(0.75f, 0.0f, 0.75f)),
      mCurveColour(vec3(1.0f, 0.5f, 0.0f)),
      mNodeSize(0.25f),
      mSelectedCp(-1),
      mUStep(0.05)

{
    auto log = spdlog::get("PathPointViewer");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("PathPointViewer");
    }
    log->info("Constructing Object");
}

PathPointViewer::~PathPointViewer
()
{
    auto log = spdlog::get("PathPointViewer");
    log->info("Destructing Object");
}

void
PathPointViewer::init
()
{
    auto log = spdlog::get("PathPointViewer");
    log->info("Initialising");
    initGLDrawable();
}

void
PathPointViewer::setPathDefinitionHandle
(PathDefinition* selected)
{
    auto log = spdlog::get("PathPointViewer");
    if (selected)
    {
        log->info("Selecting {}",selected->getNameAndUuidString());
    }
    mPathDefinitionHandle = selected;
    updateVertexBuffer();
}

void
PathPointViewer::updateVertexBuffer
()
{
    auto log = spdlog::get("PathPointViewer");
    log->info("Updating Vertex Buffer") ;
    mVertexBuffer.clear();

    if (!mPathDefinitionHandle)
    {
        log->info("No object selected");
        return;
    }

    json* controlPoints = mPathDefinitionHandle->getControlPoints();
    if (controlPoints->is_array())
    {
        for (json cp : *controlPoints)
        {
            generateNode(cp);
        }
    }
    if (controlPoints->size() > 1)
    {
        generateSpline();
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

    log->critical("Generating node cube for {} at ({},{},{})",index,x,y,z);

    vec3 colour = (mSelectedCp ==  index ? mSelectedColour : mUnselectedColour);
    log->critical("Selected? {}",mSelectedCp == index);

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
        if (mPathDefinitionHandle)
        {
            GLint modelUniform = glGetUniformLocation(mShaderProgram, "model");
            if (modelUniform == -1)
            {
                log->error("Unable to find Uniform Location for Model");
                return;
            }
            else
            {
                mat4 modelMatrix;
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
PathPointViewer::initShader
()
{
    auto log = spdlog::get("PathPointViewer");
    log->info("Initialising shader");

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

void PathPointViewer::onUpdateRequested()
{
    updateVertexBuffer();
}

void PathPointViewer::onSelectionChanged(int index)
{
   mSelectedCp = index;
   updateVertexBuffer();
}

void
PathPointViewer::generateSpline
()
{
    vector<vec3> points = generateSplinePoints();

    for (int i = 0; i<points.size()-1; i++)
    {

        LineVertex v1, v2;
        v1.Position = points.at(i);
        v1.Color = mCurveColour;
        mVertexBuffer.push_back(v1);
        v2.Position = points.at(i+1);
        v2.Color = mCurveColour;
        mVertexBuffer.push_back(v2);
    }
}

vector<vec3>
PathPointViewer::generateSplinePoints
()
{
    vector<vec3> points;
    auto log = spdlog::get("PathPointViewer");
    log->info(
        "Generating spline with {} control points",
        mPathDefinitionHandle->numberOfControlPoints()
    );

    BSpline spline(
        mPathDefinitionHandle->numberOfControlPoints(),
        3, 3, TS_CLAMPED);

    vector<tinyspline::real> ctrlp = spline.controlPoints();

    size_t i=0;
    for (json cp : *mPathDefinitionHandle->getControlPoints())
    {
        double x = cp[Constants::X];
        double y = cp[Constants::Y];
        double z = cp[Constants::Z];
        // Setup control points.
        ctrlp[i++] = x;
        ctrlp[i++] = y;
        ctrlp[i++] = z;
    }
    spline.setControlPoints(ctrlp);

    mUStep = 1.0/(mPathDefinitionHandle->numberOfControlPoints()*10);

    try
    {
        // Stores our evaluation results.
        vector<real> result;
        for (double u = 0.0; u <= 1.0; u += mUStep)
        {
            result = spline.eval(u).result();

            log->critical("Got spline point u:{}\t({},{},{})",u,
                result[0], result[1], result[2]
            );
            points.push_back(vec3(result[0], result[1], result[2]));
        }
    }
    catch (exception& ex)
    {
        log->error("Unable to generate point: {}",ex.what());
        return vector<vec3>();
    }


    log->info("Finished Loading spline points for {}",mPathDefinitionHandle->getNameAndUuidString());
    return points;
}
