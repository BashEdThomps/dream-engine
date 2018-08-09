/*
 * ScenePropertiesTreeDelegate.h
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

class ScenePropertiesModel;

class ScenePropertiesTreeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ScenePropertiesTreeDelegate(ScenePropertiesModel* model, QObject *parent = nullptr);
   ~ScenePropertiesTreeDelegate() override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public slots:
   void onButton_CaptureCameraTranslation(bool);
   void onButton_CaptureCameraRotation(bool);
   void onButton_CaptureCameraAll(bool);
    void onButton_ChooseAmbientColour(bool);
    void onButton_ChooseClearColour(bool);

signals:
   void notifyButton_CaptureCameraTranslation();
   void notifyButton_CaptureCameraRotation();
   void notifyButton_ChooseClearColour();
   void notifyButton_ChooseAmbientColour();

protected:
    ScenePropertiesModel *mModelHandle;

    QWidget* createCameraAllCaptureButton(QWidget* parent) const;
    QWidget* createCameraTranslationCaptureButton(QWidget* parent) const;
    QWidget* createCameraRotationCaptureButton(QWidget* parent) const;
    QWidget* createAmbientColourPaletteButton(QWidget* parent) const;
    QWidget* createClearColourPaletteButton(QWidget* parent) const;
};