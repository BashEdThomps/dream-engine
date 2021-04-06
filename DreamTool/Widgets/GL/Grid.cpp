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

namespace octronic::dream::tool
{
    Grid::Grid
    (DreamToolContext& p, AxisPair xp,
            float majorSpacing,float minorSpacing, float size,
            vec4 majorColour, vec4 minorColour)
        : GLWidget(p,false),
          mAxisPair(xp),
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
            TranslationColorVertex posStart, posEnd, negStart, negEnd;
            posStart.Color = mMajorColour;
            posEnd.Color = mMajorColour;
            negStart.Color = mMajorColour;
            negEnd.Color = mMajorColour;

            switch (mAxisPair)
            {
                case XZ:
                    // X
                    posStart.Translation = vec3( axis, 0.0f,-halfSize);
                    posEnd.Translation   = vec3( axis, 0.0f, halfSize);
                    negStart.Translation = vec3(-axis, 0.0f,-halfSize);
                    negEnd.Translation   = vec3(-axis, 0.0f, halfSize);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    // Z
                    posStart.Translation = vec3(-halfSize, 0.0f, axis);
                    posEnd.Translation   = vec3( halfSize, 0.0f, axis);
                    negStart.Translation = vec3(-halfSize, 0.0f,-axis);
                    negEnd.Translation   = vec3( halfSize, 0.0f,-axis);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    break;
                case YZ:
                    // Y
                    posStart.Translation = vec3(0.0f, axis,-halfSize);
                    posEnd.Translation   = vec3(0.0f, axis, halfSize);
                    negStart.Translation = vec3(0.0f,-axis,-halfSize);
                    negEnd.Translation   = vec3(0.0f,-axis, halfSize);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    // Z
                    posStart.Translation = vec3(0.0f,-halfSize, axis);
                    posEnd.Translation   = vec3(0.0f, halfSize, axis);
                    negStart.Translation = vec3(0.0f,-halfSize,-axis);
                    negEnd.Translation   = vec3(0.0f, halfSize,-axis);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    break;
                case XY:
                    // X
                    posStart.Translation = vec3( axis,-halfSize, 0.0f);
                    posEnd.Translation   = vec3( axis, halfSize, 0.0f);
                    negStart.Translation = vec3(-axis,-halfSize, 0.0f);
                    negEnd.Translation   = vec3(-axis, halfSize, 0.0f);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    // Y
                    posStart.Translation = vec3(-halfSize, axis, 0.0f);
                    posEnd.Translation   = vec3( halfSize, axis, 0.0f);
                    negStart.Translation = vec3(-halfSize,-axis, 0.0f);
                    negEnd.Translation   = vec3( halfSize,-axis, 0.0f);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
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
            TranslationColorVertex posStart, posEnd, negStart, negEnd;
            posStart.Color = mMinorColour;
            posEnd.Color   = mMinorColour;
            negStart.Color = mMinorColour;
            negEnd.Color   = mMinorColour;

            switch (mAxisPair)
            {
                case XZ:
                    // X
                    posStart.Translation = vec3( axis, 0.0f,-halfSize);
                    posEnd.Translation   = vec3( axis, 0.0f, halfSize);
                    negStart.Translation = vec3(-axis, 0.0f,-halfSize);
                    negEnd.Translation   = vec3(-axis, 0.0f, halfSize);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    // Z
                    posStart.Translation = vec3(-halfSize, 0.0f, axis);
                    posEnd.Translation   = vec3( halfSize, 0.0f, axis);
                    negStart.Translation = vec3(-halfSize, 0.0f,-axis);
                    negEnd.Translation   = vec3( halfSize, 0.0f,-axis);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    break;
                case YZ:
                    // Y
                    posStart.Translation = vec3(0.0f, axis,-halfSize);
                    posEnd.Translation   = vec3(0.0f, axis, halfSize);
                    negStart.Translation = vec3(0.0f,-axis,-halfSize);
                    negEnd.Translation   = vec3(0.0f,-axis, halfSize);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    // Z
                    posStart.Translation = vec3(0.0f,-halfSize, axis);
                    posEnd.Translation   = vec3(0.0f, halfSize, axis);
                    negStart.Translation = vec3(0.0f,-halfSize,-axis);
                    negEnd.Translation   = vec3(0.0f, halfSize,-axis);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    break;
                case XY:
                    // X
                    posStart.Translation = vec3( axis,-halfSize, 0.0f);
                    posEnd.Translation   = vec3( axis, halfSize, 0.0f);
                    negStart.Translation = vec3(-axis,-halfSize, 0.0f);
                    negEnd.Translation   = vec3(-axis, halfSize, 0.0f);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    // Y
                    posStart.Translation = vec3(-halfSize, axis, 0.0f);
                    posEnd.Translation   = vec3( halfSize, axis, 0.0f);
                    negStart.Translation = vec3(-halfSize,-axis, 0.0f);
                    negEnd.Translation   = vec3( halfSize,-axis, 0.0f);
                    addLineVertex(posStart);
                    addLineVertex(posEnd);
                    addLineVertex(negStart);
                    addLineVertex(negEnd);
                    break;
            }
        }
    }

    void Grid::initAxisLines()
    {
        vec4 red(1,0,0,1);
        vec4 green(0,1,0,1);
        vec4 blue(0,0,1,1);

        TranslationColorVertex xStart, xEnd;
        xStart.Translation = vec3(0);
        xStart.Color = red;
        xEnd.Translation = vec3(mSize/2,0,0);
        xEnd.Color = red;
        addLineVertex(xStart);
        addLineVertex(xEnd);

        TranslationColorVertex yStart, yEnd;
        yStart.Translation = vec3(0);
        yStart.Color = green;
        yEnd.Translation = vec3(0,mSize/2,0);
        yEnd.Color = green;
        addLineVertex(yStart);
        addLineVertex(yEnd);

        TranslationColorVertex zStart, zEnd;
        zStart.Translation = vec3(0);
        zStart.Color = blue;
        zEnd.Translation = vec3(0,0,mSize/2);
        zEnd.Color = blue;
        addLineVertex(zStart);
        addLineVertex(zEnd);

        TranslationColorVertex ax1HomeStart, ax1HomeEnd, ax2HomeStart, ax2HomeEnd;
        ax1HomeStart.Color = mMajorColour;
        ax1HomeEnd.Color   = mMajorColour;
        ax2HomeStart.Color = mMajorColour;
        ax2HomeEnd.Color   = mMajorColour;

        switch (mAxisPair)
        {
            case XZ:
                ax1HomeStart.Translation = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Translation   = vec3(-(mSize/2.0f),0.0f,0.0f);
                ax2HomeStart.Translation = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Translation   = vec3(0.0f,0.0f,-(mSize/2.0f));
                break;
            case YZ:
                ax1HomeStart.Translation = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Translation   = vec3(0.0f,-(mSize/2.0f),0.0f);
                ax2HomeStart.Translation = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Translation   = vec3(0.0f,0.0f,-(mSize/2.0f));
                break;
            case XY:
                ax1HomeStart.Translation = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Translation   = vec3(-(mSize/2.0f),0.0f,0.0f);
                ax2HomeStart.Translation = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Translation   = vec3(0.0f,-(mSize/2.0f),0.0f);
                break;

        }

        addLineVertex(ax1HomeStart);
        addLineVertex(ax1HomeEnd);
        addLineVertex(ax2HomeStart);
        addLineVertex(ax2HomeEnd);
    }

    Grid::AxisPair Grid::getAxisPair() const
    {
        return mAxisPair;
    }

    vec4 Grid::getMinorColour() const
    {
        return mMinorColour;
    }

    void Grid::setMinorColour(vec4 minorColour)
    {
        mMinorColour = minorColour;
    }

    void Grid::setAxisPair(Grid::AxisPair axisPair)
    {
        mAxisPair = axisPair;

    }

    vec4 Grid::getMajorColour() const
    {
        return mMajorColour;
    }

    void Grid::setMajorColour(vec4 majorColour)
    {
        mMajorColour = majorColour;
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
        clearLineVertexBuffer();
        initAxisLines();
        initMajorGridData();
        initMinorGridData();
        submitLineVertexBuffer();
    }
}
