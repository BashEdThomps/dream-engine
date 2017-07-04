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

#include <DreamCore.h>
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>

#include <vector>

using Dream::AssetType;
using Dream::Constants;
using std::vector;

AssetDefinitionPropertiesTreeDelegate::AssetDefinitionPropertiesTreeDelegate
(AssetDefinitionPropertiesModel* model, QObject* parent)
    : QItemDelegate (parent),
      mModelHandle(model)
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
    editor->setText("Model Files...");
    connect
    (
        editor,SIGNAL(clicked(bool)),
        this, SLOT(onButton_ModelFileClicked(bool))
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
        editor,SIGNAL(clicked(bool)),
        this, SLOT(onButton_ModelAdditionalFilesClicked(bool))
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
    QStringList list;
    list << QString::fromStdString(Constants::ASSET_FORMAT_SCRIPT_LUA);
    editor->addItems(list);
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
    QStringList list;
    list << QString::fromStdString(Constants::SHADER_VERTEX)
         << QString::fromStdString(Constants::SHADER_FRAGMENT);
    editor->addItems(list);
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createOpenVertexShaderInEditorButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
   QToolButton *button = new QToolButton(parent);
   button->setText("Edit Vertex Shader");
   return button;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createOpenFragmentShaderInEditorButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
   QToolButton *button = new QToolButton(parent);
   button->setText("Edit Fragment Shader");
   return button;

}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createOpenScriptInEditorButton
(AssetDefinitionPropertiesItem* adItem, QWidget* parent)
const
{
   QToolButton *button = new QToolButton(parent);
   button->setText("Edit Script");
   return button;

}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)
const
{
    AssetDefinitionPropertiesItem* adItem = static_cast<AssetDefinitionPropertiesItem*>(index.internalPointer());

    switch(adItem->getProperty())
    {
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
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
            break;
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
            break;
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
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
            break;
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_SHADER_VERTEX_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_SHADER_FRAGMENT_FILE:
            break;
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
            break;
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
AssetDefinitionPropertiesTreeDelegate::onButton_ModelFileButtonClicked
(bool checked)
{
    qDebug() << "ModelFileBrowseDelegate: Browse was clicked"
             << checked;
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_ModelAdditionalFilesClicked
(bool checked)
{
    qDebug() << "BrowseForAdditionalFilesDelegate: Browse was clicked"
             << checked;
}
