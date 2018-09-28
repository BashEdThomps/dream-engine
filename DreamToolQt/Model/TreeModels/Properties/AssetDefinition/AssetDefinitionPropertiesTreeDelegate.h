/*
 * AssetDefinitionTreeDelegate.h
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

#include "AssetDefinitionPropertiesItem.h"

class AssetDefinitionPropertiesModel;
class TemplatesModel;
class QComboBox;

class AssetDefinitionPropertiesTreeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    AssetDefinitionPropertiesTreeDelegate(AssetDefinitionPropertiesModel* model, QObject *parent = nullptr);
    ~AssetDefinitionPropertiesTreeDelegate() override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const  override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const  QModelIndex &index) const  override;

public slots:
    void onButton_PathList(bool clicked);
    void onButton_AudioFile(bool clicked);
    void onButton_AudioEvents(bool clicked);
    void onButton_FontFile(bool clicked);
    void onButton_ModelFile(bool clicked);
    void onButton_ModelAdditionalFiles(bool clicked);
    void onButton_RemoveFiles(bool clicked);
    void onButton_EditShader(bool clicked);
    void onButton_EditScript(bool clicked);
    void onButton_PhysicsBvhTriangleMeshFile(bool clicked);
    void onButton_FontColourDialog(bool clicked) ;
    void onButton_ModelMaterialShaderMap(bool clicked);

    void onButton_LightChooseAmbient(bool clicked);
    void onButton_LightChooseDiffuse(bool clicked);
    void onButton_LightChooseSpecular(bool clicked);

signals:
    void notifyButton_PathList();
    void notifyButton_AudioFile();
    void notifyButton_AudioEvents();
    void notifyButton_FontFile();
    void notifyButton_ModelFile();
    void notifyButton_ModelAdditionalFiles();
    void notifyButton_RemoveFiles();

    void notifyButton_EditScript();
    void notifyButton_EditShader();

    void notifyButton_PhysicsBvhTriangleMeshFile();
    void notifyButton_ModelMaterialShaderMap();


    void notifyButton_LightChooseAmbient();
    void notifyButton_LightChooseDiffuse();
    void notifyButton_LightChooseSpecular();

protected:
    AssetDefinitionPropertiesModel *mModelHandle;

    QWidget* createFormatComboBox(AssetDefinitionPropertiesItem *item, QWidget *parent = nullptr) const;
    QWidget* createTypeComboBox(AssetDefinitionPropertiesItem *item, QWidget *parent = nullptr)  const;
    QWidget* createFontFileButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget* createModelFileButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget* createAudioEventsButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget* createAudioFileButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget* createModelAdditionalFilesButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget* createOpenShaderInEditorButton(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget* createOpenScriptInEditorButton(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget* createRemoveFilesButton(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget* createPhysicsBvhTriangleMeshFileButton(AssetDefinitionPropertiesItem*, QWidget* parent) const;
    QWidget* createFontColourDialogButton(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget* createModelMaterialShaderButton(AssetDefinitionPropertiesItem* adItem, QWidget *parent) const;
    QWidget* createLightAmbientPaletteButton(QWidget* parent) const;
    QWidget* createLightDiffusePaletteButton(QWidget* parent) const;
    QWidget* createLightSpecularPaletteButton(QWidget* parent) const;
    QWidget* createPathListButton(AssetDefinitionPropertiesItem*, QWidget* parent) const;
    QWidget* createPathTypeComboBox(AssetDefinitionPropertiesItem* assetDef, QWidget* parent) const;

};
