/*
 * ProjectTreeItem.h
 *
 * Created: 15 2017 by Ashley
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

#include "../GenericTreeItem.h"
#include <QList>
#include <QVariant>

enum ScenegraphTreeItemType
{
    SCENEGRAPH_PROJECT,
    SCENEGRAPH_SCENE,
    SCENEGRAPH_SCENE_OBJECT,
    SCENEGRAPH_TREE_NODE
};

class ScenegraphTreeItem : public GenericTreeItem
{
public:
    explicit ScenegraphTreeItem
    (
        QString title,
        ScenegraphTreeItemType type,
        void* item,
        ScenegraphTreeItem* parentItem = nullptr
    );

    ~ScenegraphTreeItem();

    void* getItem() const;
    ScenegraphTreeItemType getType() const;
    QVariant data(int column) const;
private:
    void* mItem;
    ScenegraphTreeItemType mType;
};
