/*
 * OpenInTextEditorDelegate.h
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

class OpenInTextEditorDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    OpenInTextEditorDelegate(AssetDefinition* adHandle, QObject* parent = nullptr);
    ~OpenInTextEditorDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void notifyOpenInTextEditorButtonClicked(AssetDefinition*);

public slots:
    void onOpenButtonClicked(bool checked);

private:
    AssetDefinition* mAssetDefinitionHandle;
};
