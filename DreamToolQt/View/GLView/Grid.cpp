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
#include <spdlog/spdlog.h>

using namespace glm;
using namespace std;
using namespace Dream;

Grid::Grid
(
    QObject* parent,
    float majorSpacing, float minorSpacing, float size,
    vec3 majorColour, vec3 minorColour
)
    : GLDrawable(parent),

      mSize(size),
      mMajorSpacing(majorSpacing),
      mMajorColour(majorColour),
      mMinorSpacing(minorSpacing),
      mMinorColour(minorColour)

{
    auto log = spdlog::get("Grid");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("Grid");
    }
    log->info("Constructing with majorSpacing: {}, minorSpacing {}", mMajorSpacing, minorSpacing);
}

Grid::~Grid
()
{
    auto log = spdlog::get("Grid");
    log->info("Destructing");
}

void
Grid::init
()
{
    initGLDrawable();
    initMinorGridData();
    initMajorGridData();
    initAxisLines();
}

void
Grid::initMajorGridData
()
{

    auto log = spdlog::get("Grid");
    log->info("Init Major Data");
    float halfSize = (mSize/2.0f);

    // Major Grid
    for (float z = -halfSize; z <= halfSize; z += mMajorSpacing)
    {
        LineVertex majorStart, majorEnd;

        majorStart.Position = vec3(-halfSize, 0.0f, z);
        majorStart.Color = mMajorColour;

        majorEnd.Position = vec3(halfSize, 0.0f, z);
        majorEnd.Color = mMajorColour;

        mVertexBuffer.push_back(majorStart);
        mVertexBuffer.push_back(majorEnd);
    }

    for (float x = -halfSize; x <= halfSize; x += mMajorSpacing)
    {
        LineVertex majorStart, majorEnd;

        majorStart.Position = vec3(x, 0.0f, -halfSize);
        majorStart.Color = mMajorColour;

        majorEnd.Position = vec3(x, 0.0f, halfSize);
        majorEnd.Color = mMajorColour;

        mVertexBuffer.push_back(majorStart);
        mVertexBuffer.push_back(majorEnd);
    }
}

void
Grid::initMinorGridData
()
{
    auto log = spdlog::get("Grid");
    log->info("Init Minor Data");
    float halfSize = (mSize/2.0f);

    // Minor Grid
    for (float z = -halfSize; z <= halfSize; z += mMinorSpacing)
    {

        if (static_cast<int>(z) % static_cast<int>(mMajorSpacing) == 0) continue;

        LineVertex minorStart, minorEnd;

        minorStart.Position = vec3(-halfSize, 0.0f, z);
        minorStart.Color = mMinorColour;

        minorEnd.Position = vec3(halfSize, 0.0f, z);
        minorEnd.Color = mMinorColour;

        mVertexBuffer.push_back(minorStart);
        mVertexBuffer.push_back(minorEnd);
    }

    for (float x = -halfSize; x <= halfSize; x += mMinorSpacing)
    {

        if (static_cast<int>(x) % static_cast<int>(mMajorSpacing) == 0) continue;

        LineVertex minorStart, minorEnd;

        minorStart.Position = vec3(x, 0.0f, -halfSize);
        minorStart.Color = mMinorColour;

        minorEnd.Position = vec3(x, 0.0f, halfSize);
        minorEnd.Color = mMinorColour;

        mVertexBuffer.push_back(minorStart);
        mVertexBuffer.push_back(minorEnd);
    }
}

void Grid::initAxisLines()
{
    vec3 red(1,0,0);
    vec3 green(0,1,0);
    vec3 blue(0,0,1);

    LineVertex xStart, xEnd;
    xStart.Position = vec3(0);
    xStart.Color = red;
    xEnd.Position = vec3(mSize,0,0);
    xEnd.Color = red;
    mVertexBuffer.push_back(xStart);
    mVertexBuffer.push_back(xEnd);


    LineVertex yStart, yEnd;
    yStart.Position = vec3(0);
    yStart.Color = green;
    yEnd.Position = vec3(0,mSize,0);
    yEnd.Color = green;
    mVertexBuffer.push_back(yStart);
    mVertexBuffer.push_back(yEnd);

    LineVertex zStart, zEnd;
    zStart.Position = vec3(0);
    zStart.Color = blue;
    zEnd.Position = vec3(0,0,mSize);
    zEnd.Color = blue;
    mVertexBuffer.push_back(zStart);
    mVertexBuffer.push_back(zEnd);
}

float
Grid::getMajorSpacing
()
{
   return mMajorSpacing;
}

float
Grid::getMinorSpacing
()
{
    return mMinorSpacing;
}
