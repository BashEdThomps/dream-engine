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
(QObject* parent, float majorSpacing,  float minorSpacing)
    : QObject(parent),
      mMajorSpacing(majorSpacing),
      mMinorSpacing(minorSpacing)
{

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

}
