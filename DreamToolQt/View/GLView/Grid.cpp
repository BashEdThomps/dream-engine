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
#include <QDebug>

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
    {
        cout << "Grid: Constructing with majorSpacing: " << mMajorSpacing
             << ", minorSpacing: " << mMinorSpacing << endl;
    }
}

Grid::~Grid
()
{
    {
        cout << "Grid: Destructing" << endl;
    }
}

void
Grid::init
()
{
    initGLDrawable();
    initMinorGridData();
    initMajorGridData();
}

void
Grid::initMajorGridData
()
{

    qDebug() << "Grid: Init Major Data";
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
    qDebug() << "Grid: Init Minor Data";
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
