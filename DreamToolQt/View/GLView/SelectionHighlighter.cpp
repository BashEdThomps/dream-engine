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
#include <QDebug>

SelectionHighlighter::SelectionHighlighter
(QObject *parent)
    : GLDrawable(parent),
      mSelectedObjectHandle(nullptr),
      mColour(glm::vec3(0.0f,1.0f,0.0f)),
      mOffset(0.1f)
{
    qDebug() << "SelectionHighlighter: Constructing Object";
}

SelectionHighlighter::~SelectionHighlighter
()
{
    qDebug() << "SelectionHighlighter: Destructing Object";
}

void
SelectionHighlighter::init
()
{
    qDebug() << "SelectionHighlighter: Initialising";
    initGLDrawable();
}

void
SelectionHighlighter::setSelectedObject
(SceneObject* selected)
{
    qDebug() << "SelectionHighlighter: Selecting "
             << QString::fromStdString(selected->getNameAndUuidString());
    mSelectedObjectHandle = selected;
    updateVertexBuffer();
}

void
SelectionHighlighter::updateVertexBuffer
()
{
    qDebug() << "SelectionHighlighter: Updating Vertex Buffer" ;
    mVertexBuffer.clear();

    BoundingBox bounds;

    if (mSelectedObjectHandle->hasModelInstance())
    {
        bounds = mSelectedObjectHandle->getModelInstance()->getBoundingBox();
    }

    qDebug() << "SelectionHighlighter: Minimum Bounds "
             << bounds.minimum.x << ","
             << bounds.minimum.y << ","
             << bounds.minimum.z;

    qDebug() << "SelectionHighlighter: Maximum Bounds "
             << bounds.maximum.x << ","
             << bounds.maximum.y << ","
             << bounds.maximum.z;

    // Top Quad

    LineVertex topFront1, topFront2;
    topFront1.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    topFront1.Color = mColour;
    mVertexBuffer.push_back(topFront1);
    topFront2.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    topFront2.Color = mColour;
    mVertexBuffer.push_back(topFront2);

    LineVertex topBack1, topBack2;
    topBack1.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    topBack1.Color = mColour;
    mVertexBuffer.push_back(topBack1);
    topBack2.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    topBack2.Color = mColour;
    mVertexBuffer.push_back(topBack2);

    LineVertex topLeft1, topLeft2;
    topLeft1.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    topLeft1.Color = mColour;
    mVertexBuffer.push_back(topLeft1);
    topLeft2.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    topLeft2.Color = mColour;
    mVertexBuffer.push_back(topLeft2);

    LineVertex topRight1, topRight2;
    topRight1.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    topRight1.Color = mColour;
    mVertexBuffer.push_back(topRight1);
    topRight2.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    topRight2.Color = mColour;
    mVertexBuffer.push_back(topRight2);

    // Bottom Quad

    LineVertex bottomFront1, bottomFront2;
    bottomFront1.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    bottomFront1.Color = mColour;
    mVertexBuffer.push_back(bottomFront1);
    bottomFront2.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    bottomFront2.Color = mColour;
    mVertexBuffer.push_back(bottomFront2);

    LineVertex bottomBack1, bottomBack2;
    bottomBack1.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    bottomBack1.Color = mColour;
    mVertexBuffer.push_back(bottomBack1);
    bottomBack2.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    bottomBack2.Color = mColour;
    mVertexBuffer.push_back(bottomBack2);

    LineVertex bottomLeft1, bottomLeft2;
    bottomLeft1.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    bottomLeft1.Color = mColour;
    mVertexBuffer.push_back(bottomLeft1);
    bottomLeft2.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    bottomLeft2.Color = mColour;
    mVertexBuffer.push_back(bottomLeft2);

    LineVertex bottomRight1, bottomRight2;
    bottomRight1.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    bottomRight1.Color = mColour;
    mVertexBuffer.push_back(bottomRight1);
    bottomRight2.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    bottomRight2.Color = mColour;
    mVertexBuffer.push_back(bottomRight2);

    // Verticals

    LineVertex frontLeft1, frontLeft2;
    frontLeft1.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    frontLeft1.Color = mColour;
    mVertexBuffer.push_back(frontLeft1);
    frontLeft2.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    frontLeft2.Color = mColour;
    mVertexBuffer.push_back(frontLeft2);

    LineVertex frontRight1, frontRight2;
    frontRight1.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.minimum.z-mOffset);
    frontRight1.Color = mColour;
    mVertexBuffer.push_back(frontRight1);
    frontRight2.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.minimum.z-mOffset);
    frontRight2.Color = mColour;
    mVertexBuffer.push_back(frontRight2);

    LineVertex backLeft1, backLeft2;
    backLeft1.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    backLeft1.Color = mColour;
    mVertexBuffer.push_back(backLeft1);
    backLeft2.Position = glm::vec3(bounds.minimum.x-mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    backLeft2.Color = mColour;
    mVertexBuffer.push_back(backLeft2);

    LineVertex backRight1, backRight2;
    backRight1.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.minimum.y-mOffset, bounds.maximum.z+mOffset);
    backRight1.Color = mColour;
    mVertexBuffer.push_back(backRight1);
    backRight2.Position = glm::vec3(bounds.maximum.x+mOffset, bounds.maximum.y+mOffset, bounds.maximum.z+mOffset);
    backRight2.Color = mColour;
    mVertexBuffer.push_back(backRight2);
}

void
SelectionHighlighter::draw
()
{
    if (!mVertexBuffer.empty())
    {
        glEnable(GL_LINE_SMOOTH);
        qDebug() << "SelectionHighlighter: Drawing all - " << mVertexBuffer.size()/2 << " lines.";

        // Enable shader program
        glUseProgram(mShaderProgram);

        // Set the projection matrix
        GLint projUniform = glGetUniformLocation(mShaderProgram, "projection");
        if (projUniform == -1)
        {
            cerr << "SelectionHighlighter: Unable to find Uniform Location for projection" << endl;
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
            cerr << "SelectionHighlighter: Unable to find Uniform Location for view" << endl;
            return;
        }
        else
        {
            glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
        }

        // Set the model matrix
        if (mSelectedObjectHandle)
        {
            GLint modelUniform = glGetUniformLocation(mShaderProgram, "model");
            if (modelUniform == -1)
            {
                cerr << "SelectionHighlighter: Unable to find Uniform Location for Model" << endl;
                return;
            }
            else
            {
                glm::mat4 modelMatrix;
                if (mSelectedObjectHandle->hasModelInstance())
                {
                   modelMatrix = mSelectedObjectHandle->getModelInstance()->getModelMatrix();
                }
                else
                {
                    // Get raw data
                    glm::vec3 translation = mSelectedObjectHandle->getTranslation();
                    glm::quat rot = mSelectedObjectHandle->getTransform()->getOrientation();
                    glm::vec3 scale = mSelectedObjectHandle->getScale();
                    // Translate
                    modelMatrix = glm::translate(modelMatrix,translation);
                    // Rotate
                    glm::mat4 rotMat = glm::mat4_cast(rot);
                    modelMatrix = modelMatrix * rotMat;
                    // Scale
                    modelMatrix = glm::scale(modelMatrix, scale);
                }
                glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
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
        glDisable(GL_LINE_SMOOTH);
    }
}

void
SelectionHighlighter::initShader
()
{
    qDebug() << "SelectionHighlighter: Initialising shader";

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
        cerr << "SelectionHighlighter: SHADER:VERTEX:COMPILATION_FAILED\n" << infoLog << endl;
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
        cerr << "SelectionHighlighter: SHADER:FRAGMENT:COMPILATION_FAILED\n" << infoLog << endl;
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
        cerr << "SelectionHighlighter: SHADER:PROGRAM:LINKING_FAILED\n" << infoLog << endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

SceneObject*
SelectionHighlighter::getSelectedObject
()
{
    return mSelectedObjectHandle;
}
