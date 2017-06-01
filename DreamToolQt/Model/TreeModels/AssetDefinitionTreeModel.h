/*
 * AssetDefinitionTreeModel.h
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

#ifndef ASSET_DEFINITION_TREEMODEL_H
#define ASSET_DEFINITION_TREEMODEL_H

#include <QAbstractItemModel>
#include "AssetDefinitionTreeItem.h"
#include <DreamCore.h>

class AssetDefinitionTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AssetDefinitionTreeModel(Dream::Project *project, QObject *parent = 0);
    ~AssetDefinitionTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    void setupModelData();
    Dream::Project *mProject;
    AssetDefinitionTreeItem *mRootItem;
};

#endif // ASSET_DEFINITION_TREEMODEL_H