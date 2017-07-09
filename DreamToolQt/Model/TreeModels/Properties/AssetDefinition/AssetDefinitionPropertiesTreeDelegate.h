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
    AssetDefinitionPropertiesTreeDelegate(TemplatesModel* tpModel, AssetDefinitionPropertiesModel* model, QObject *parent = nullptr);
    ~AssetDefinitionPropertiesTreeDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public slots:
    void onButton_AudioFile(bool);
    void onButton_FontFile(bool);
    void onButton_ModelFile(bool);
    void onButton_ModelAdditionalFiles(bool);
    void onButton_RemoveFiles(bool clicked);
    void onButton_EditVertexShader(bool clicked);
    void onButton_EditFragmentShader(bool clicked);
    void onButton_EditScript(bool clicked);
    void onButton_PhysicsBvhTriangleMeshFile(bool clicked);
    void onCombo_ScriptTemplateChanged(const QString&);
    void onCombo_ShaderTemplateChanged(const QString&);

signals:
    void notifyButton_AudioFile();
    void notifyButton_FontFile();
    void notifyButton_ModelFile();
    void notifyButton_ModelAdditionalFiles();
    void notifyButton_RemoveFiles();

    void notifyButton_EditScript();
    void notifyButton_EditFragmentShader();
    void notifyButton_EditVertexShader();

    void notifyButton_PhysicsBvhTriangleMeshFile();

    void notifyCombo_ScriptTemplateChanged(const QString&);
    void notifyCombo_ShaderTemplateChanged(const QString&);


protected:
    AssetDefinitionPropertiesModel *mModelHandle;
    TemplatesModel* mTemplatesModelHandle;

    QWidget *createFormatComboBox(AssetDefinitionPropertiesItem *item, QWidget *parent = nullptr) const;
    QWidget *createTypeComboBox(AssetDefinitionPropertiesItem *item, QWidget *parent = nullptr) const ;
    QWidget *createFontFileButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget *createModelFileButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget *createAudioFileButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget *createModelAdditionalFilesButton(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget *createScriptTemplateComboBox(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget *createShaderTemplateComboBox(AssetDefinitionPropertiesItem *adItem, QWidget *parent) const;
    QWidget *createOpenVertexShaderInEditorButton(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget *createOpenFragmentShaderInEditorButton(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget *createOpenScriptInEditorButton(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget *createRemoveFilesButton(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget *createTemplateComboBox(AssetDefinitionPropertiesItem* adItem, QWidget* parent) const;
    QWidget *createPhysicsBvhTriangleMeshFileButton(AssetDefinitionPropertiesItem*, QWidget* parent) const;
};
