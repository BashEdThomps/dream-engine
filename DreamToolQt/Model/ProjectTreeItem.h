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
#ifndef PROJECTTREEITEM_H
#define PROJECTTREEITEM_H

#include <QList>
#include <QVariant>

enum ProjectItemType
{
    PROJECT,
    ASSET_DEFINITION,
    SCENE,
    SCENE_OBJECT,
    TREE_NODE
};


class ProjectTreeItem
{
public:
    explicit ProjectTreeItem(const QList<QVariant> &data,ProjectItemType type, ProjectTreeItem *parentItem = 0);
    ~ProjectTreeItem();
    void appendChild(ProjectTreeItem *child);
    ProjectTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    ProjectTreeItem *parentItem();
    ProjectItemType getItemType();
private:
    QList<ProjectTreeItem*> mChildItems;
    QList<QVariant> mItemData;
    ProjectTreeItem *mParentItem;
    ProjectItemType mType;
};

#endif // PROJECTTREEITEM_H
