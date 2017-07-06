/*
 * AssetDefinitionTreeDelegate.cpp
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
#include "AssetDefinitionPropertiesTreeDelegate.h"

#include "AssetDefinitionPropertiesItem.h"
#include "AssetDefinitionPropertiesModel.h"
#include "../../../TemplatesModel.h"

#include <DreamCore.h>
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>
#include <QHBoxLayout>
#include <QStandardItemModel>

#include <vector>

using Dream::AssetType;
using Dream::Constants;
using std::vector;

AssetDefinitionPropertiesTreeDelegate::AssetDefinitionPropertiesTreeDelegate
(TemplatesModel* templatesModel, AssetDefinitionPropertiesModel* model, QObject* parent)
    : QItemDelegate (parent),
      mModelHandle(model),
      mTemplatesModelHandle(templatesModel)
{
    qDebug() << "AssetDefinitionTreeDelegate: Constructing";
}

AssetDefinitionPropertiesTreeDelegate::~AssetDefinitionPropertiesTreeDelegate
()
{
    qDebug() << "AssetDefinitionTreeDelegate: Destructing";
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createTypeComboBox(AssetDefinitionPropertiesItem *item, QWidget* parent)
const
{
    QComboBox *editor = new QComboBox(parent);
    QStringList list ;
    for (std::pair<AssetType,std::string> type : Constants::DREAM_ASSET_TYPES_MAP)
    {
        list << QString::fromStdString(type.second);
    }
    editor->addItems(list);
    string type = item->getAssetDefinitionHandle()->getType();
    int typeIndex = editor->findText(QString::fromStdString(type));
    editor->setCurrentIndex(typeIndex);
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createFormatComboBox(AssetDefinitionPropertiesItem* item, QWidget* parent)
const
{
    qDebug() << "AssetDefinitionTreeDelegate: createEditor";
    QComboBox *editor = new QComboBox(parent);
    editor->setDuplicatesEnabled(false);
    editor->setEditable(false);
    editor->setInsertPolicy(QComboBox::NoInsert);

    AssetType type = Constants::assetTypeFromString(item->getAssetDefinitionHandle()->getType());
    // Setup List
    vector<string> formats = Constants::DREAM_ASSET_FORMATS_MAP[type];
    QStringList list;
    for (string format : formats)
    {
        list.push_back(QString::fromStdString(format));
    }
    editor->addItems(list);

    string format = item->getAssetDefinitionHandle()->getFormat();
    int formatIndex = editor->findText(QString::fromStdString(format));
    editor->setCurrentIndex(formatIndex);
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createModelFileButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
    QToolButton *editor = new QToolButton(parent);
    editor->setText("Model File...");
    connect
    (
        editor,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_ModelFile(bool))
    );
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createModelAdditionalFilesButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
    QToolButton *editor = new QToolButton(parent);
    editor->setText("Additional Files...");
    connect
    (
        editor,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_ModelAdditionalFiles(bool))
    );
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createScriptTemplateComboBox
(AssetDefinitionPropertiesItem* adItem, QWidget *parent)
const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setDuplicatesEnabled(false);
    editor->setEditable(false);
    editor->setMinimumHeight(25);

    QString selectPrompt = "Select Template";
    editor->addItem(selectPrompt);
    int promptIndex = editor->findText(selectPrompt);
    qobject_cast<QStandardItemModel*>
    (
        editor->model()
    )->item(promptIndex)->setEnabled(false);

    QStringList templates = mTemplatesModelHandle->getScriptTemplateNames();
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: Got script templates"
             << templates;
    editor->addItems(templates);

    editor->setCurrentIndex(promptIndex);

    connect
    (
        editor,
        SIGNAL(currentTextChanged(const QString&)),
        this,
        SLOT(onCombo_ScriptTemplateChanged(const QString&))
    );
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createShaderTemplateComboBox
(AssetDefinitionPropertiesItem* adItem, QWidget *parent)
const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setDuplicatesEnabled(false);
    editor->setEditable(false);
    editor->setMinimumHeight(25);

    QString selectPrompt = "Select Template";
    editor->addItem(selectPrompt);
    int promptIndex = editor->findText(selectPrompt);
    qobject_cast<QStandardItemModel*>
    (
        editor->model()
    )->item(promptIndex)->setEnabled(false);

    QStringList templates = mTemplatesModelHandle->getShaderTemplateNames();
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: Got shader templates"
             << templates;
    editor->addItems(templates);

    editor->setCurrentIndex(promptIndex);

    connect
    (
        editor,
        SIGNAL(currentTextChanged(const QString&)),
        this,
        SLOT(onCombo_ShaderTemplateChanged(const QString&))
    );
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createOpenVertexShaderInEditorButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
   QToolButton *button = new QToolButton(parent);
   button->setText("Edit Vertex Shader");
   connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_EditVertexShader(bool))
    );
   return button;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createOpenFragmentShaderInEditorButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
   QToolButton *button = new QToolButton(parent);
   button->setText("Edit Fragment Shader");
   connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_EditFragmentShader(bool))
    );
   return button;

}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createOpenScriptInEditorButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
    QToolButton *button = new QToolButton(parent);
    button->setText("Edit Script");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_EditScript(bool))
    );
    return button;

}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createRemoveFilesButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
    QToolButton *button = new QToolButton(parent);
    button->setText("Remove Files");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_RemoveFiles(bool))
    );
    return button;

}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createTemplateComboBox
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
    if (adItem->getAssetDefinitionHandle()->isTypeScript())
    {
        return createScriptTemplateComboBox(adItem,parent);
    }
    else if (adItem->getAssetDefinitionHandle()->isTypeShader())
    {
        return createShaderTemplateComboBox(adItem,parent);
    }
    return new QComboBox();
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)
const
{
    AssetDefinitionPropertiesItem* adItem = static_cast<AssetDefinitionPropertiesItem*>(index.internalPointer());

    switch(adItem->getProperty())
    {
        case ASSET_DEFINITION_PROPERTY_TEMPLATE:
            return createTemplateComboBox(adItem,parent);
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
            return createRemoveFilesButton(adItem, parent);
        case ASSET_DEFINITION_PROPERTY_NAME:
            return new QLineEdit(parent);
        case ASSET_DEFINITION_PROPERTY_TYPE:
            return createTypeComboBox(adItem, parent);
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            return createFormatComboBox(adItem,parent);
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
            return createModelFileButton(adItem,parent);
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
            return createModelAdditionalFilesButton(adItem,parent);
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
            return createOpenScriptInEditorButton(adItem,parent);
        case ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE:
            return createOpenVertexShaderInEditorButton(adItem,parent);
        case ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE:
            return createOpenFragmentShaderInEditorButton(adItem,parent);
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_NONE:
            return new QLineEdit(parent);
    }
    return new QLineEdit(parent);
}

void
AssetDefinitionPropertiesTreeDelegate::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    AssetDefinitionPropertiesItem* adItem = static_cast<AssetDefinitionPropertiesItem*>(index.internalPointer());
    QString value = index.model()->data(index, Qt::DisplayRole).toString();

    switch(adItem->getProperty())
    {
        case ASSET_DEFINITION_PROPERTY_NAME:
            static_cast<QLineEdit*>(editor)->setText(value);
            break;
        case ASSET_DEFINITION_PROPERTY_TYPE:
            static_cast<QComboBox*>(editor)->addItem(index.model()->data(index, Qt::DisplayRole).toString());
            break;
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            static_cast<QComboBox*>(editor)->setCurrentText(value);
            break;

        case ASSET_DEFINITION_PROPERTY_TEMPLATE:
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_NONE:
            break;
    }
}

void
AssetDefinitionPropertiesTreeDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    AssetDefinitionPropertiesItem* adItem = static_cast<AssetDefinitionPropertiesItem*>(index.internalPointer());
    switch(adItem->getProperty())
    {
        case ASSET_DEFINITION_PROPERTY_NAME:
            model->setData(index,static_cast<QLineEdit*>(editor)->text());
            break;
        case ASSET_DEFINITION_PROPERTY_TYPE:
            model->setData(index,static_cast<QComboBox*>(editor)->currentText());
            break;
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            model->setData(index,static_cast<QComboBox*>(editor)->currentText());
            break;
        case ASSET_DEFINITION_PROPERTY_TEMPLATE:
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_NONE:
            break;
    }
}

void
AssetDefinitionPropertiesTreeDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)
const
{
    editor->setGeometry(option.rect);
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_ModelFile
(bool clicked)
{
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: ModelFile was clicked";
    emit notifyButton_ModelFile();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_ModelAdditionalFiles
(bool clicked)
{
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: ModelAdditionalFiles was clicked";
    emit notifyButton_ModelAdditionalFiles();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_RemoveFiles
(bool clicked)
{
   qDebug() << "AssetDefinitionPropertiesTreeDelegate: RemoveFiles was clicked";
   emit notifyButton_RemoveFiles();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_EditVertexShader
(bool clicked)
{
   qDebug() << "AssetDefinitionPropertiesTreeDelegate: EditVertexShader was clicked";
   emit notifyButton_EditVertexShader();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_EditFragmentShader
(bool clicked)
{
   qDebug() << "AssetDefinitionPropertiesTreeDelegate: EditFragmentShader was clicked";
   emit notifyButton_EditFragmentShader();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_EditScript
(bool clicked)
{
   qDebug() << "AssetDefinitionPropertiesTreeDelegate: EditScript was clicked";
   emit notifyButton_EditScript();
}

void
AssetDefinitionPropertiesTreeDelegate::onCombo_ScriptTemplateChanged
(const QString& templateName)
{

    qDebug() << "AssetDefinitionPropertiesTreeDelegate: Script Template"
             << templateName << "selected";
    emit notifyCombo_ScriptTemplateChanged(templateName);
}

void
AssetDefinitionPropertiesTreeDelegate::onCombo_ShaderTemplateChanged
(const QString& templateName)
{

    qDebug() << "AssetDefinitionPropertiesTreeDelegate: Shader Template"
             << templateName << "selected";
    emit notifyCombo_ShaderTemplateChanged(templateName);
}


