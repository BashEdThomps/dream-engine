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
namespace DreamTool
{

    Grid::Grid
    (
            Project* p,
            AxisPair xp,
            glm::vec3 position,
            float majorSpacing,
            float minorSpacing,
            float size,
            vec3 majorColour,
            vec3 minorColour
            )
        : GLWidget(p),
          mAxisPair(xp),
          mTranslation(position),
          mSize(size),
          mMajorSpacing(majorSpacing),
          mMinorSpacing(minorSpacing),
          mMajorColour(majorColour),
          mMinorColour(minorColour)

    {
        setLogClassName("Grid");
        auto log = getLog();
        log->debug("Constructing with majorSpacing: {}, minorSpacing {}", mMajorSpacing, minorSpacing);
    }

    Grid::~Grid
    ()
    {
        auto log = getLog();
        log->debug("Destructing");
    }

    void
    Grid::init
    ()
    {
        GLWidget::init();
        recalculateGridLines();
    }

    void
    Grid::initMajorGridData
    ()
    {
        auto log = getLog();
        log->debug("Init Major Data");
        float halfSize = (mSize/2.0f);

        // Major Grid
        for (float axis1 = -halfSize; axis1 <= halfSize; axis1 += mMajorSpacing)
        {
            GLWidgetVertex majorStart, majorEnd;

            switch (mAxisPair)
            {
                case XZ:
                    majorStart.Position = vec3(axis1, 0.0f, -halfSize);
                    majorEnd.Position = vec3(axis1, 0.0f, (axis1 != 0.0f ?halfSize:0.0f));
                    majorStart.Color = mMajorColour;
                    majorEnd.Color = mMajorColour;
                    break;
                case YZ:
                    majorStart.Position = vec3(0.0f, axis1, -halfSize);
                    majorEnd.Position = vec3(0.0f, axis1, (axis1 != 0.0f ?halfSize:0.0f));
                    majorStart.Color = mMajorColour;
                    majorEnd.Color = mMajorColour;
                    break;
                case XY:
                    majorStart.Position = vec3(axis1, -halfSize, 0.0f);
                    majorEnd.Position = vec3(axis1, (axis1 != 0.0f ?halfSize:0.0f), 0.0f);
                    majorStart.Color = mMajorColour;
                    majorEnd.Color = mMajorColour;
                    break;

            }
            mVertexBuffer.push_back(majorStart);
            mVertexBuffer.push_back(majorEnd);
        }

        for (float axis2 = -halfSize; axis2 <= halfSize; axis2 += mMajorSpacing)
        {
            GLWidgetVertex majorStart, majorEnd;
            switch (mAxisPair)
            {
                case XZ:
                    majorStart.Position = vec3(-halfSize,0.0f,axis2);
                    majorEnd.Position = vec3((axis2 != 0.0f ?halfSize:0.0f),0.0f,axis2);
                    majorStart.Color = mMajorColour;
                    majorEnd.Color = mMajorColour;
                    break;
                case YZ:
                    majorStart.Position = vec3(0.0f,  -halfSize, axis2);
                    majorEnd.Position = vec3(0.0f, (axis2 != 0.0f ?halfSize:0.0f), axis2);
                    majorStart.Color = mMajorColour;
                    majorEnd.Color = mMajorColour;
                    break;
                case XY:
                    majorStart.Position = vec3(-halfSize,axis2,0.0f);
                    majorEnd.Position = vec3((axis2 != 0.0f ?halfSize:0.0f),axis2,0.0f);
                    majorStart.Color = mMajorColour;
                    majorEnd.Color = mMajorColour;
                    break;
            }
            mVertexBuffer.push_back(majorStart);
            mVertexBuffer.push_back(majorEnd);
        }
    }

    void
    Grid::initMinorGridData
    ()
    {
        auto log = getLog();
        log->debug("Init Minor Data");
        float halfSize = (mSize/2.0f);

        // Minor Grid
        for (float axis1 = -halfSize; axis1 <= halfSize; axis1 += mMinorSpacing)
        {
            if (static_cast<int>(axis1) % static_cast<int>(mMajorSpacing) == 0) continue;

            GLWidgetVertex minorStart, minorEnd;
            switch(mAxisPair)
            {
                case XY:
                    minorStart.Position = vec3(axis1, -halfSize, 0.0f);
                    minorEnd.Position = vec3(axis1, (axis1 != 0.0f ?halfSize:0.0f), 0.0f);
                    minorStart.Color = mMinorColour;
                    minorEnd.Color = mMinorColour;
                    break;
                case XZ:
                    minorStart.Position = vec3(axis1, 0.0f, -halfSize);
                    minorEnd.Position = vec3(axis1, 0.0f, (axis1 != 0.0f ?halfSize:0.0f));
                    minorStart.Color = mMinorColour;
                    minorEnd.Color = mMinorColour;
                    break;
                case YZ:
                    minorStart.Position = vec3(0.0f,  axis1,-halfSize);
                    minorEnd.Position = vec3(0.0f,  axis1, (axis1 != 0.0f ?halfSize:0.0f));
                    minorStart.Color = mMinorColour;
                    minorEnd.Color = mMinorColour;
                    break;
            }


            mVertexBuffer.push_back(minorStart);
            mVertexBuffer.push_back(minorEnd);
        }

        for (float axis2 = -halfSize; axis2 <= halfSize; axis2 += mMinorSpacing)
        {

            if (static_cast<int>(axis2) % static_cast<int>(mMajorSpacing) == 0) continue;

            GLWidgetVertex minorStart, minorEnd;

            switch (mAxisPair)
            {
                case XY:
                    minorStart.Position = vec3(-halfSize,axis2,0.0f);
                    minorEnd.Position = vec3((axis2 != 0.0f ? halfSize:0.0f),axis2,0.0f);
                    minorStart.Color = mMinorColour;
                    minorEnd.Color = mMinorColour;
                    break;
                case XZ:
                    minorStart.Position = vec3(-halfSize, 0.0f, axis2);
                    minorEnd.Position = vec3((axis2 != 0.0f ?halfSize:0.0f),0.0f,axis2);
                    minorStart.Color = mMinorColour;
                    minorEnd.Color = mMinorColour;
                    break;
                case YZ:
                    minorStart.Position = vec3( 0.0f, -halfSize, axis2);
                    minorEnd.Position = vec3(0.0f, (axis2 != 0.0f ?halfSize:0.0f), axis2);
                    minorStart.Color = mMinorColour;
                    minorEnd.Color = mMinorColour;
                    break;
            }

            mVertexBuffer.push_back(minorStart);
            mVertexBuffer.push_back(minorEnd);
        }
    }

    void Grid::initAxisLines()
    {
        vec3 red(1,0,0);
        vec3 green(0,1,0);
        vec3 blue(0,0,1);

        GLWidgetVertex xStart, xEnd;
        xStart.Position = vec3(0);
        xStart.Color = red;
        xEnd.Position = vec3(mSize/2,0,0);
        xEnd.Color = red;
        mVertexBuffer.push_back(xStart);
        mVertexBuffer.push_back(xEnd);

        GLWidgetVertex yStart, yEnd;
        yStart.Position = vec3(0);
        yStart.Color = green;
        yEnd.Position = vec3(0,mSize/2,0);
        yEnd.Color = green;
        mVertexBuffer.push_back(yStart);
        mVertexBuffer.push_back(yEnd);

        GLWidgetVertex zStart, zEnd;
        zStart.Position = vec3(0);
        zStart.Color = blue;
        zEnd.Position = vec3(0,0,mSize/2);
        zEnd.Color = blue;
        mVertexBuffer.push_back(zStart);
        mVertexBuffer.push_back(zEnd);
    }

    Grid::AxisPair Grid::getAxisPair() const
    {
        return mAxisPair;
    }

    vec3 Grid::getMinorColour() const
    {
        return mMinorColour;
    }

    void Grid::setMinorColour(vec3 minorColour)
    {
        mMinorColour = minorColour;
    }

    void Grid::setAxisPair(Grid::AxisPair axisPair)
    {
        mAxisPair = axisPair;

    }

    vec3 Grid::getMajorColour() const
    {
        return mMajorColour;
    }

    void Grid::setMajorColour(vec3 majorColour)
    {
        mMajorColour = majorColour;
    }

    glm::vec3 Grid::getTranslation() const
    {
        return mTranslation;
    }

    void Grid::setTranslation(vec3 translation)
    {
        mTranslation = translation;
        mModelMatrix = glm::translate(mat4(1.0f),mTranslation);
    }

    float
    Grid::getMajorSpacing
    ()
    {
        return mMajorSpacing;
    }

    void
    Grid::setMajorSpacing
    (float ms)
    {
        mMajorSpacing = ms;
    }

    float
    Grid::getMinorSpacing
    ()
    {
        return mMinorSpacing;
    }

    void
    Grid::setMinorSpacing
    (float ms)
    {
        mMinorSpacing = ms;
    }

    float Grid::getSize()
    {
        return mSize;
    }

    void Grid::setSize(float sz)
    {
        mSize = sz;
    }

    void Grid::recalculateGridLines()
    {
        mVertexBuffer.clear();
        initMinorGridData();
        initMajorGridData();
        initAxisLines();

        // Vertex Array
        glBindVertexArray(mVao);
        ShaderInstance::CurrentVAO = mVao;
        checkGLError();

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        ShaderInstance::CurrentVBO = mVbo;
        checkGLError();

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(GLWidgetVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
        checkGLError();

        glBindVertexArray(0);
    }
}
