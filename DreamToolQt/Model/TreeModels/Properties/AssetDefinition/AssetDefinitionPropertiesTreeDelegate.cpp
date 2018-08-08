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
#include <QCheckBox>
#include <QToolButton>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QSpinBox>
#include <QColorDialog>

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
AssetDefinitionPropertiesTreeDelegate::createFontColourDialogButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
const
{
    QToolButton *editor = new QToolButton(parent);
    editor->setText("Pick Colour");
    connect
    (
        editor,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_FontColourDialog(bool))
    );
    return editor;
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_FontColourDialog
(bool)
{
   QColorDialog *editor = new QColorDialog();
   editor->show();
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createTypeComboBox
(AssetDefinitionPropertiesItem *item, QWidget* parent)
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
AssetDefinitionPropertiesTreeDelegate::createFormatComboBox
(AssetDefinitionPropertiesItem* item, QWidget* parent)
const
{
    qDebug() << "AssetDefinitionTreeDelegate: createEditor";
    QComboBox *editor = new QComboBox(parent);
    editor->setDuplicatesEnabled(false);
    editor->setEditable(false);
    editor->setInsertPolicy(QComboBox::NoInsert);

    AssetType type = Constants::getAssetTypeEnumFromString(item->getAssetDefinitionHandle()->getType());
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
AssetDefinitionPropertiesTreeDelegate::createAudioFileButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
const
{
    QToolButton *editor = new QToolButton(parent);
    editor->setText("Audio File...");
    connect
    (
        editor,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_AudioFile(bool))
    );
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createFontFileButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
const
{
    QToolButton *editor = new QToolButton(parent);
    editor->setText("Font File...");
    connect
    (
        editor,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_FontFile(bool))
    );
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createModelMaterialShaderButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
const
{
    QToolButton *editor = new QToolButton(parent);
    editor->setText("Edit Map...");
    connect
    (
        editor,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_ModelMaterialShaderMap(bool))
    );
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createModelFileButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
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
AssetDefinitionPropertiesTreeDelegate::createPhysicsBvhTriangleMeshFileButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
const
{
    QToolButton *editor = new QToolButton(parent);
    editor->setText("Mesh File...");
    connect
    (
        editor,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_PhysicsBvhTriangleMeshFile(bool))
    );
    return editor;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createModelAdditionalFilesButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
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
AssetDefinitionPropertiesTreeDelegate::createOpenShaderInEditorButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
const
{
   QToolButton *button = new QToolButton(parent);
   button->setText("Edit Shader");
   connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_EditShader(bool))
    );
   return button;
}

QWidget*
AssetDefinitionPropertiesTreeDelegate::createOpenScriptInEditorButton
(AssetDefinitionPropertiesItem*, QWidget* parent)
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
(AssetDefinitionPropertiesItem*, QWidget* parent)
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
AssetDefinitionPropertiesTreeDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex &index)
const
{
    AssetDefinitionPropertiesItem* adItem = static_cast<AssetDefinitionPropertiesItem*>(index.internalPointer());
    QDoubleSpinBox *spinBox = nullptr;

    switch(adItem->getProperty())
    {
            // QCheckBox
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
        case ASSET_DEFINITION_PROPERTY_AUDIO_LOOP:
            return new QCheckBox(parent);

            // QDoubleSpinBox all range
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
            spinBox = new QDoubleSpinBox(parent);
            spinBox->setRange(numeric_limits<double>::lowest(), numeric_limits<double>::max());
            return spinBox;

            // QDoubleSpinBox - 0 to max
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
            spinBox = new QDoubleSpinBox(parent);
            spinBox->setRange(0.0, numeric_limits<double>::max());
            return spinBox;

            //QDoubleSpinBox 0.0 - 1.0
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA:
            spinBox = new QDoubleSpinBox(parent);
            spinBox->setRange(0.0,1.0);
            spinBox->setDecimals(3);
            return spinBox;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE:
            return createPhysicsBvhTriangleMeshFileButton(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
            return createRemoveFilesButton(adItem, parent);

        case ASSET_DEFINITION_PROPERTY_TYPE:
            return createTypeComboBox(adItem, parent);

        case ASSET_DEFINITION_PROPERTY_FORMAT:
            return createFormatComboBox(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
            return createAudioFileButton(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
            return createFontFileButton(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
            return createModelFileButton(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
            return createModelAdditionalFilesButton(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_MODEL_MATERIAL_SHADER_TABLE:
            return createModelMaterialShaderButton(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
            return createOpenScriptInEditorButton(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_SHADER_FILES:
            return createOpenShaderInEditorButton(adItem,parent);

        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR:
            return createFontColourDialogButton(adItem,parent);

            // Not Used
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL:
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILDREN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
        case ASSET_DEFINITION_PROPERTY_NAME:
        case ASSET_DEFINITION_PROPERTY_NONE:
            return new QLineEdit(parent);
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR:
            return createLightColourPaletteButton(parent);
    }
    return new QLineEdit(parent);
}


QWidget* AssetDefinitionPropertiesTreeDelegate::createLightColourPaletteButton(QWidget* parent) const
{
    QToolButton* button = new QToolButton(parent);
    button->setText("Choose Colour...");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_LightChooseColour(bool))
    );
    return button;
}

void AssetDefinitionPropertiesTreeDelegate::onButton_LightChooseColour(bool clicked)
{
    Q_UNUSED(clicked)
    emit notifyButton_LightChooseColour();
}

void AssetDefinitionPropertiesTreeDelegate::onButton_ModelMaterialShaderMap(bool clicked)
{
   Q_UNUSED(clicked)
   emit notifyButton_ModelMaterialShaderMap();
}

void
AssetDefinitionPropertiesTreeDelegate::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    AssetDefinitionPropertiesItem* adItem = static_cast<AssetDefinitionPropertiesItem*>(index.internalPointer());
    QVariant value = index.model()->data(index, Qt::DisplayRole);

    switch(adItem->getProperty())
    {
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
        case ASSET_DEFINITION_PROPERTY_AUDIO_LOOP:
            static_cast<QCheckBox*>(editor)->setChecked(value.toBool());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA:
            static_cast<QDoubleSpinBox*>(editor)->setValue(value.toDouble());
            break;

        case ASSET_DEFINITION_PROPERTY_NAME:
            static_cast<QLineEdit*>(editor)->setText(value.toString());
            break;

        case ASSET_DEFINITION_PROPERTY_TYPE:
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            static_cast<QComboBox*>(editor)->setCurrentText(value.toString());
            break;

        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILDREN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
        case ASSET_DEFINITION_PROPERTY_MODEL_MATERIAL_SHADER_TABLE:
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_FILES:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_NONE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR:
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
        // QCheckBox
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_KINEMATIC:
        case ASSET_DEFINITION_PROPERTY_AUDIO_LOOP:
            model->setData(index,static_cast<QCheckBox*>(editor)->isChecked());
            break;

        // QDoubleSpinBox
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_CONSTANT:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_X:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Y:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL_Z:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_RADIUS:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MASS:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_MARGIN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_X:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Y:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS_Z:
        case ASSET_DEFINITION_PROPERTY_FONT_SIZE:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_RED:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_GREEN:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR_BLUE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_RED:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_GREEN:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_BLUE:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR_ALPHA:
            model->setData(index,static_cast<QDoubleSpinBox*>(editor)->value());
            break;

        // QLineEdit
        case ASSET_DEFINITION_PROPERTY_NAME:
            model->setData(index,static_cast<QLineEdit*>(editor)->text());
            break;

        // QComboBox
        case ASSET_DEFINITION_PROPERTY_TYPE:
        case ASSET_DEFINITION_PROPERTY_FORMAT:
            model->setData(index,static_cast<QComboBox*>(editor)->currentText());
            break;

        // Not Used
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_BVH_TRIANGLE_MESH_FILE:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILDREN:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_COMPOUND_CHILD:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_NORMAL:
        case ASSET_DEFINITION_PROPERTY_PHYSICS_OBJECT_HALF_EXTENTS:
        case ASSET_DEFINITION_PROPERTY_FONT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_LIGHT_COLOUR:
        case ASSET_DEFINITION_PROPERTY_REMOVE_FILES:
        case ASSET_DEFINITION_PROPERTY_ANIMATION_FILE:
        case ASSET_DEFINITION_PROPERTY_AUDIO_FILE:
        case ASSET_DEFINITION_PROPERTY_FONT_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_FILE:
        case ASSET_DEFINITION_PROPERTY_MODEL_ADDITIONAL_FILES:
        case ASSET_DEFINITION_PROPERTY_MODEL_MATERIAL_SHADER_TABLE:
        case ASSET_DEFINITION_PROPERTY_SCRIPT_FILE:
        case ASSET_DEFINITION_PROPERTY_SHADER_FILES:
        case ASSET_DEFINITION_PROPERTY_SPRITE_FILE:
        case ASSET_DEFINITION_PROPERTY_NONE:
            break;
    }
}

void
AssetDefinitionPropertiesTreeDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&)
const
{
    editor->setGeometry(option.rect);
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_AudioFile
(bool)
{
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: AudioFile was clicked";
    emit notifyButton_AudioFile();
}


void
AssetDefinitionPropertiesTreeDelegate::onButton_FontFile
(bool)
{
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: FontFile was clicked";
    emit notifyButton_FontFile();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_ModelFile
(bool)
{
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: ModelFile was clicked";
    emit notifyButton_ModelFile();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_PhysicsBvhTriangleMeshFile
(bool)
{
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: PhysicsBvhTriangleMeshFile was clicked";
    emit notifyButton_PhysicsBvhTriangleMeshFile();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_ModelAdditionalFiles
(bool)
{
    qDebug() << "AssetDefinitionPropertiesTreeDelegate: ModelAdditionalFiles was clicked";
    emit notifyButton_ModelAdditionalFiles();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_RemoveFiles
(bool)
{
   qDebug() << "AssetDefinitionPropertiesTreeDelegate: RemoveFiles was clicked";
   emit notifyButton_RemoveFiles();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_EditShader
(bool)
{
   qDebug() << "AssetDefinitionPropertiesTreeDelegate: EditShader was clicked";
   emit notifyButton_EditShader();
}

void
AssetDefinitionPropertiesTreeDelegate::onButton_EditScript
(bool)
{
   qDebug() << "AssetDefinitionPropertiesTreeDelegate: EditScript was clicked";
   emit notifyButton_EditScript();
}




