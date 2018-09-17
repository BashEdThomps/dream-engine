/*
 * PathPointViewer.h
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
#pragma once

#include "GLDrawable.h"
#include <DreamCore.h>

using glm::vec3;
using Dream::PathDefinition;
using Dream::PathInstance;

class PathPointViewer : public GLDrawable
{
    Q_OBJECT
public:
    explicit PathPointViewer(QObject *parent = nullptr);
    ~PathPointViewer() override;

    void setPathDefinition(PathDefinition* object);
    void init() override;
    void draw() override;
    void initShader() override;

    vector<vec3> generateSplinePoints();
    void setPathVisible(bool visible);

    size_t getTangentIndex() ;
    void setTangentIndex( size_t& tangentIndex);

    bool getDrawTangent() ;
    void setDrawTangent(bool drawTangent);
    void clearRuntime();

public slots:
    void onUpdateRequested();
    void onSelectionChanged(int);
    void onTangentIndexChanged(int);
    void onTangentVisibilityChanged(bool);

private:
    void generateNode(json cp);

signals:
    void notifyNumberOfTangentsChanged(int);

private:
    PathDefinition* mPathDefinition;
    unique_ptr<PathInstance> mPathInstance;
    void updateVertexBuffer();
    vec3 mSelectedColour;
    vec3 mUnselectedColour;
    vec3 mCurveColour;
    vec3 mTangentColour;
    size_t mTangentIndex;
    bool mDrawTangent;
    float mNodeSize;
    int mSelectedCp;
    double mUStep;
    void generateSpline();
    bool mVisible;
};
