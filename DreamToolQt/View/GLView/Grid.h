/*
 * Grid.h
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
#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

using namespace std;

enum Mode
{
    SELECTION,
    TRANSLATION,
    ROTATION,
    SCALE
};

class Grid : public QObject
{
public:
    Grid(QObject* parent,  float majorSpacing = 10.0f, float minorSpacing = 1.0f);
    ~Grid();
    void generateGLData();
    void generateGridData();
    void draw();
private: // Member Functions
    void drawTranslationHandles();
    void drawRotationHandles();
    void drawScaleHandles();
    void drawSelectionOutline();
private: // Variables
    float mMajorSpacing;
    float mMinorSpacing;
    glm::vec3 mXColour;
    glm::vec3 mYColour;
    glm::vec3 mZColour;
    GLuint mVertexArray;
    GLuint mVertexBuffer;
    vector<GLfloat> mVertecies;
    vector<GLfloat> mIndicies;
};

#endif // GRID_H
