/*
 * RelationshipTree.h
 *
 * Created: 10 2017 by Ashley
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

#ifndef RELATIONSHIPTREE_H
#define RELATIONSHIPTREE_H

#include "GLDrawable.h"
#include <DreamCore.h>

using namespace Dream;

class RelationshipTree : public GLDrawable
{
public:
    RelationshipTree(QObject* parent);
    ~RelationshipTree() override;
    void init() override;
    void setRootSceneObjectHandle(SceneObject*);
    void setLeafSceneObjectHandle(SceneObject*);
    void clearRuntime();
protected:
    vec3 mEdgeColour;
    vec3 mNodeColour;
    SceneObject* mRootSceneObjectHandle;
    SceneObject* mLeafSceneObjectHandle;
};

#endif // RELATIONSHIPTREE_H
