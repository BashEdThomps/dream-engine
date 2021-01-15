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
#include <glm/gtc/matrix_transform.hpp>
#include <DreamCore.h>

using namespace Dream;
namespace DreamTool
{

    Grid::Grid
    (
            DTContext* p,
            AxisPair xp,
            glm::vec3 position,
            float majorSpacing,
            float minorSpacing,
            float size,
            vec3 majorColour,
            vec3 minorColour
            )
        : GLWidget(p,false),
          mAxisPair(xp),
          mTranslation(position),
          mSize(size),
          mMajorSpacing(majorSpacing),
          mMinorSpacing(minorSpacing),
          mMajorColour(majorColour),
          mMinorColour(minorColour)

    {
        LOG_DEBUG("Grid: Constructing with majorSpacing: {}, minorSpacing {}", mMajorSpacing, minorSpacing);
    }

    Grid::~Grid
    ()
    {
        LOG_DEBUG("Grid: Destructing");
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
        LOG_DEBUG("Grid: Init Major Data");
        float halfSize = mSize/2.0f;

        // Major Grid
        for (float axis = mMajorSpacing; axis <= halfSize; axis += mMajorSpacing)
        {
            GLWidgetVertex posStart, posEnd, negStart, negEnd;
            posStart.Color = mMajorColour;
            posEnd.Color = mMajorColour;
            negStart.Color = mMajorColour;
            negEnd.Color = mMajorColour;

            switch (mAxisPair)
            {
                case XZ:
                    // X
                    posStart.Position = vec3( axis, 0.0f,-halfSize);
                    posEnd.Position   = vec3( axis, 0.0f, halfSize);
                    negStart.Position = vec3(-axis, 0.0f,-halfSize);
                    negEnd.Position   = vec3(-axis, 0.0f, halfSize);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    // Z
                    posStart.Position = vec3(-halfSize, 0.0f, axis);
                    posEnd.Position   = vec3( halfSize, 0.0f, axis);
                    negStart.Position = vec3(-halfSize, 0.0f,-axis);
                    negEnd.Position   = vec3( halfSize, 0.0f,-axis);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    break;
                case YZ:
                    // Y
                    posStart.Position = vec3(0.0f, axis,-halfSize);
                    posEnd.Position   = vec3(0.0f, axis, halfSize);
                    negStart.Position = vec3(0.0f,-axis,-halfSize);
                    negEnd.Position   = vec3(0.0f,-axis, halfSize);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    // Z
                    posStart.Position = vec3(0.0f,-halfSize, axis);
                    posEnd.Position   = vec3(0.0f, halfSize, axis);
                    negStart.Position = vec3(0.0f,-halfSize,-axis);
                    negEnd.Position   = vec3(0.0f, halfSize,-axis);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    break;
                case XY:
                    // X
                    posStart.Position = vec3( axis,-halfSize, 0.0f);
                    posEnd.Position   = vec3( axis, halfSize, 0.0f);
                    negStart.Position = vec3(-axis,-halfSize, 0.0f);
                    negEnd.Position   = vec3(-axis, halfSize, 0.0f);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    // Y
                    posStart.Position = vec3(-halfSize, axis, 0.0f);
                    posEnd.Position   = vec3( halfSize, axis, 0.0f);
                    negStart.Position = vec3(-halfSize,-axis, 0.0f);
                    negEnd.Position   = vec3( halfSize,-axis, 0.0f);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    break;
            }
        }
    }

    void
    Grid::initMinorGridData
    ()
    {
        LOG_DEBUG("Grid: Init Minor Data");
        float halfSize = (mSize/2.0f);

        // Major Grid
        for (float axis = mMinorSpacing; axis <= halfSize; axis += mMinorSpacing)
        {
            GLWidgetVertex posStart, posEnd, negStart, negEnd;
            posStart.Color = mMinorColour;
            posEnd.Color   = mMinorColour;
            negStart.Color = mMinorColour;
            negEnd.Color   = mMinorColour;

            switch (mAxisPair)
            {
                case XZ:
                    // X
                    posStart.Position = vec3( axis, 0.0f,-halfSize);
                    posEnd.Position   = vec3( axis, 0.0f, halfSize);
                    negStart.Position = vec3(-axis, 0.0f,-halfSize);
                    negEnd.Position   = vec3(-axis, 0.0f, halfSize);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    // Z
                    posStart.Position = vec3(-halfSize, 0.0f, axis);
                    posEnd.Position   = vec3( halfSize, 0.0f, axis);
                    negStart.Position = vec3(-halfSize, 0.0f,-axis);
                    negEnd.Position   = vec3( halfSize, 0.0f,-axis);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    break;
                case YZ:
                    // Y
                    posStart.Position = vec3(0.0f, axis,-halfSize);
                    posEnd.Position   = vec3(0.0f, axis, halfSize);
                    negStart.Position = vec3(0.0f,-axis,-halfSize);
                    negEnd.Position   = vec3(0.0f,-axis, halfSize);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    // Z
                    posStart.Position = vec3(0.0f,-halfSize, axis);
                    posEnd.Position   = vec3(0.0f, halfSize, axis);
                    negStart.Position = vec3(0.0f,-halfSize,-axis);
                    negEnd.Position   = vec3(0.0f, halfSize,-axis);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    break;
                case XY:
                    // X
                    posStart.Position = vec3( axis,-halfSize, 0.0f);
                    posEnd.Position   = vec3( axis, halfSize, 0.0f);
                    negStart.Position = vec3(-axis,-halfSize, 0.0f);
                    negEnd.Position   = vec3(-axis, halfSize, 0.0f);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    // Y
                    posStart.Position = vec3(-halfSize, axis, 0.0f);
                    posEnd.Position   = vec3( halfSize, axis, 0.0f);
                    negStart.Position = vec3(-halfSize,-axis, 0.0f);
                    negEnd.Position   = vec3( halfSize,-axis, 0.0f);
                    mVertexBuffer.push_back(posStart);
                    mVertexBuffer.push_back(posEnd);
                    mVertexBuffer.push_back(negStart);
                    mVertexBuffer.push_back(negEnd);
                    break;
            }
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

        GLWidgetVertex ax1HomeStart, ax1HomeEnd, ax2HomeStart, ax2HomeEnd;
        ax1HomeStart.Color = mMajorColour;
        ax1HomeEnd.Color   = mMajorColour;
        ax2HomeStart.Color = mMajorColour;
        ax2HomeEnd.Color   = mMajorColour;

        switch (mAxisPair)
        {
            case XZ:
                ax1HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Position   = vec3(-(mSize/2.0f),0.0f,0.0f);
                ax2HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Position   = vec3(0.0f,0.0f,-(mSize/2.0f));
                break;
            case YZ:
                ax1HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Position   = vec3(0.0f,-(mSize/2.0f),0.0f);
                ax2HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Position   = vec3(0.0f,0.0f,-(mSize/2.0f));
                break;
            case XY:
                ax1HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Position   = vec3(-(mSize/2.0f),0.0f,0.0f);
                ax2HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Position   = vec3(0.0f,-(mSize/2.0f),0.0f);
                break;

        }

        mVertexBuffer.push_back(ax1HomeStart);
        mVertexBuffer.push_back(ax1HomeEnd);
        mVertexBuffer.push_back(ax2HomeStart);
        mVertexBuffer.push_back(ax2HomeEnd);
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
        mMajorSpacing = ms < 1.0f ? 1.0f : ms;
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
        mMinorSpacing = ms < 0.1f ? 0.1f : ms;
    }

    float Grid::getSize()
    {
        return mSize;
    }

    void Grid::setSize(float sz)
    {
        mSize = sz < 1.0f ? 1.0f : sz;
    }

    void Grid::recalculateGridLines()
    {
        mVertexBuffer.clear();
        initAxisLines();
        initMajorGridData();
        initMinorGridData();

        // Vertex Array
        glBindVertexArray(mVao);
        ShaderRuntime::CurrentVAO = mVao;
        GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        ShaderRuntime::CurrentVBO = mVbo;
        GLCheckError();
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(GLWidgetVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
        GLCheckError();
        glBindVertexArray(0);
    }
}
