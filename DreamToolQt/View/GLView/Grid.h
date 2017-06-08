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

#include <QOpenGLShaderProgram>

using namespace std;
using namespace glm;

struct GridVertex
{
    vec3 Position;
    vec3 Color;
};

class Grid : public QObject
{
    Q_OBJECT
public:
    Grid(
        QObject* parent,
        float majorSpacing = 5.0f, float minorSpacing = 1.0f,
        float size = 25.0f,
        vec3 majorColour = vec3(1.0f,0.0f,0.0f),
        vec3 minorColour = vec3(0.0f,0.0f,1.0f)
    );

    ~Grid();

    void init();
    bool isInitialised();
    void draw();
    void setViewMatrix(mat4);
    void setProjectionMatrix(mat4);

private: // Member Functions
    void initShader();
    void initVaoVbo();
    void initMajorGridData();
    void initMinorGridData();

private: // Variables

    float mSize;

    mat4 mViewMatrix;
    mat4 mProjectionMatrix;

    float mMajorSpacing;
    vec3 mMajorColour;

    float mMinorSpacing;
    vec3 mMinorColour;

    GLuint mVao;
    GLuint mVbo;

    vector<GridVertex> mVertexBuffer;

    GLuint mShaderProgram;

    bool mInitialised;
};

#endif // GRID_H
