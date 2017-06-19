/*
 * GLDrawable.h
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
#ifndef GLDRAWABLE_H
#define GLDRAWABLE_H

#include <QObject>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace std;
using namespace glm;

struct LineVertex
{
    vec3 Position;
    vec3 Color;
};

class GLDrawable : public QObject
{
    Q_OBJECT
public:
    explicit GLDrawable(QObject *parent = nullptr);
    ~GLDrawable();
    bool isInitialised();
    virtual void draw();
    void setViewMatrix(mat4);
    void setProjectionMatrix(mat4);
    virtual void init() = 0;

signals:

public slots:

protected: // Inherited Functions
    void initGLDrawable();

private: // Member Functions
    virtual void initShader();
    virtual void initVaoVbo();

protected: // Variables
    mat4 mViewMatrix;
    mat4 mProjectionMatrix;
    GLuint mVao;
    GLuint mVbo;
    vector<LineVertex> mVertexBuffer;
    GLuint mShaderProgram;
    bool mInitialised;
};

#endif // GLDRAWABLE_H
