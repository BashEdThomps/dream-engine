/*
 * BrowseForAdditionalFilesDelegate.h
 *
 * Created: 27 2017 by Ashley
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

#include <QItemDelegate>

namespace Dream
{
    class AssetDefinition;
}

using Dream::AssetDefinition;

class BrowseForAdditionalFilesDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    BrowseForAdditionalFilesDelegate(AssetDefinition* adHandle, QObject* parent = nullptr);
    ~BrowseForAdditionalFilesDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    void notifyBrowseButtonClicked(AssetDefinition*);
public slots:
    void onBrowseButtonClicked(bool checked);
private:
    AssetDefinition* mAssetDefinitionHandle;
};
