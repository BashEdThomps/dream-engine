/*
 * ProjectTreeModel.h
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

#include <QAbstractItemModel>
#include "ScenegraphTreeItem.h"
#include <DreamCore.h>
#include <memory>
#include <QIcon>

using std::unique_ptr;
using Dream::SceneObjectDefinition;
using Dream::ProjectDefinition;

class ScenegraphTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ScenegraphTreeModel(ProjectDefinition *project, QObject *parent = nullptr);
    ~ScenegraphTreeModel() override;

    QVariant data(const QModelIndex &index, int role) const  override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,  const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const  override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::DropActions supportedDropActions() const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList& indexes) const override;

    void setupModelData();
    void forceDataChanged(const QModelIndex& index);

     static QString SO_MIME_TYPE;

signals:
    void notifyExpandRequested();

private:
    void appendSceneObjects(SceneObjectDefinition *parentSceneObject, ScenegraphTreeItem* parentTreeNode);
    ProjectDefinition *mProjectDefinitionHandle;
    unique_ptr<ScenegraphTreeItem> mRootItem;
    unique_ptr<QIcon> mProjectIcon;
    unique_ptr<QIcon> mSceneIcon;
    unique_ptr<QIcon> mSceneObjectIcon;
};



