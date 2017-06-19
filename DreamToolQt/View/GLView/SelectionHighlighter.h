/*
 * SelectionHighlighter.h
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
using Dream::SceneObjectRuntime;
using Dream::BoundingBox;

class SelectionHighlighter : public GLDrawable
{
public:
    explicit SelectionHighlighter(QObject *parent = 0);
    ~SelectionHighlighter();

    void setSelectedObject(SceneObjectRuntime* object);
    SceneObjectRuntime* getSelectedObject();
    void init() override;
    void draw() override;
    void initShader() override;

private:
    SceneObjectRuntime* mSelectedObjectHandle;
    void updateVertexBuffer();
    vec3 mSelectionColour;
    vec3 mXColour;
    vec3 mYColour;
    vec3 mZColour;
    float mOffset;
};
