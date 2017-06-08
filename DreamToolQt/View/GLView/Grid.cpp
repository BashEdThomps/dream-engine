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
      mMinorColour(minorColour)

{
    generateGLData();
    generateGridData();
}

Grid::~Grid
()
{
    glDeleteVertexArrays(1,&mVertexArray);
    glDeleteBuffers(1,&mVertexBuffer);
}

void
Grid::draw
()
{
    // Use Shader
    // Fill Buffers
    glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
    // Pass Uniforms
    // Pass View/Proj
    // Draw Lines
    // Detatch Buffers
    glBindVertexArray(0);
    // Detatch Shader
    glUseProgram(0);
}

void
Grid::initShader
()
{

}

void
Grid::generateGLData
()
{
    // Create Vertex Attrib Array
    glGenVertexArrays(1,&mVertexArray);
    // Create Data Buffer
    glGenBuffers(1,&mVertexBuffer);
}
void
Grid::generateGridData
()
{
    generateMajorGridData();
    generateMinorGridData();

    //glBindVertexArray(mVertexArray);
    //glBufferData(...);
}

void
Grid::generateMajorGridData
()
{
    float halfSize = (mSize/2.0f);

    // Major Grid
    for (float y = -halfSize; y <= halfSize; y += mMajorSpacing)
    {
        mMajorGridVertecies.push_back(vec3(-halfSize,y,0.0f));
        mMajorGridVertecies.push_back(vec3(halfSize,y,0.0f));
    }

    for (float x = -halfSize; x <= halfSize; x += mMajorSpacing)
    {
        mMajorGridVertecies.push_back(vec3(x,-halfSize,0.0f));
        mMajorGridVertecies.push_back(vec3(x,halfSize,0.0f));
    }
}

void
Grid::generateMinorGridData
()
{
    float halfSize = (mSize/2.0f);

    // Major Grid
    for (float y = -halfSize; y <= halfSize; y += mMinorSpacing)
    {
        mMajorGridVertecies.push_back(vec3(-halfSize,y,0.0f));
        mMajorGridVertecies.push_back(vec3(halfSize,y,0.0f));
    }

    for (float x = -halfSize; x <= halfSize; x += mMinorSpacing)
    {
        mMajorGridVertecies.push_back(vec3(x,-halfSize,0.0f));
        mMajorGridVertecies.push_back(vec3(x,halfSize,0.0f));
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
