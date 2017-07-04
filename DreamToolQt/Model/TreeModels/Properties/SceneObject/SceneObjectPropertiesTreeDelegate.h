/*
 * SceneObjectPropertiesTreeDelegate.h
 *
 * Created: 03 2017 by Ashley
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

class SceneObjectPropertiesModel;

class SceneObjectPropertiesTreeDelegate : public QItemDelegate
{
public:
    SceneObjectPropertiesTreeDelegate(SceneObjectPropertiesModel* model, QObject* parent = nullptr);
    ~SceneObjectPropertiesTreeDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    SceneObjectPropertiesModel *mModelHandle;

    QWidget* createCaptureTranslationButton(QWidget* parent) const;
    QWidget* createCaptureRotationButton(QWidget* parent) const;
    QWidget* createCaptureScaleButton(QWidget* parent) const;
    QWidget* createTransformTypeComboBox(QWidget* parent) const;
    QWidget* createRemoveAssetDefinitionButton(QWidget* parent) const;
    QWidget* createRemoveChildButton(QWidget* parent) const;
};
