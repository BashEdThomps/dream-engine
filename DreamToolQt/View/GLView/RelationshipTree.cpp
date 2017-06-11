/*
 * RelationshipTree.cpp
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
#include "RelationshipTree.h"

RelationshipTree::RelationshipTree
(QObject* parentHandle)
    : GLDrawable(parentHandle),
      mEdgeColour(vec3(0.0f,0.0f,1.0f)),
      mNodeColour(vec3(1.0f,0.0f,0.0f)),
      mRootSceneObjectHandle(nullptr),
      mLeafSceneObjectHandle(nullptr)
{
    if (Constants::DEBUG)
    {
        cout << "RelationshipTree: Constructing Object";
    }
}

RelationshipTree::~RelationshipTree
()
{
    if (Constants::DEBUG)
    {
        cout << "RelationshipTree: Denstructing Object";
    }
}

void
RelationshipTree::init
()
{
    initGLDrawable();
}

void
RelationshipTree::setRootSceneObjectHandle
(SceneObject* root)
{
    mRootSceneObjectHandle = root;
}

void
RelationshipTree::setLeafSceneObjectHandle
(SceneObject* root)
{
    mLeafSceneObjectHandle = root;
}
