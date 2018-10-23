/*
 * SceneObjectPropertiesTreeDelegate.cpp
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
#include "SceneObjectPropertiesTreeDelegate.h"

#include <DreamCore.h>
#include <QItemDelegate>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QToolButton>
#include "../../TreeModelToolButton.h"

#include "SceneObjectPropertiesModel.h"
#include "SceneObjectPropertiesItem.h"

using Dream::Constants;
using std::numeric_limits;

SceneObjectPropertiesTreeDelegate::SceneObjectPropertiesTreeDelegate
(SceneObjectPropertiesModel* model, QObject* parent)
    : QItemDelegate (parent),
      mModelHandle(model)

{
    auto log = spdlog::get("SceneObjectPropertiesTreeDelegate");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("SceneObjectPropertiesTreeDelegate");
    }
    log->trace("Constructing");
}

SceneObjectPropertiesTreeDelegate::~SceneObjectPropertiesTreeDelegate
()
{
    auto log = spdlog::get("SceneObjectPropertiesTreeDelegate");
    log->trace("Destructing");
}

QWidget*
SceneObjectPropertiesTreeDelegate::createEditor
(QWidget *parent, const  QStyleOptionViewItem &option, const QModelIndex &index)
const
{
    SceneObjectPropertiesItem* sopItem = static_cast<SceneObjectPropertiesItem*>(index.internalPointer());
    QDoubleSpinBox* spinbox = nullptr;
    switch (sopItem->getProperty())
    {
        case SCENE_OBJECT_PROPERTY_CHILD:
            return createRemoveChildButton(sopItem->getTargetSceneObjectDefinitionHandle(),parent);
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION:
            return createRemoveAssetDefinitionButton(sopItem->getTargetAssetDefinitionHandle(),parent);
        case SCENE_OBJECT_PROPERTY_TRANSLATION_CAPTURE:
            return createCaptureTranslationButton(parent);
        case SCENE_OBJECT_PROPERTY_ORIENTATION_CAPTURE:
            return createCaptureOrientationButton(parent);
        case SCENE_OBJECT_PROPERTY_SCALE_CAPTURE:
            return createCaptureScaleButton(parent);
        case SCENE_OBJECT_PROPERTY_UUID:
        case SCENE_OBJECT_PROPERTY_NAME:
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION_LIST:
        case SCENE_OBJECT_PROPERTY_CHILD_LIST:
            return new QLineEdit(parent);
        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_W:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_X:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Y:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Z:
        case SCENE_OBJECT_PROPERTY_SCALE_X:
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            spinbox = new QDoubleSpinBox(parent);
            spinbox->setRange(numeric_limits<double>::lowest(), numeric_limits<double>::max());
            spinbox->setDecimals(3);
            return spinbox;
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            return createTransformTypeComboBox(parent);
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
        case SCENE_OBJECT_PROPERTY_FOLLOWS_CAMERA:
        case SCENE_OBJECT_PROPERTY_ALWAYS_DRAW:
        case SCENE_OBJECT_PROPERTY_STATIC:
            return new QCheckBox(parent);
        case SCENE_OBJECT_PROPERTY_NONE:
            return new QLineEdit(parent);
    }
    return new QLineEdit(parent);
}

void
SceneObjectPropertiesTreeDelegate::setEditorData
(QWidget *editor,const  QModelIndex &index)
const
{
    QVariant value = index.model()->data(index, Qt::DisplayRole);

    SceneObjectPropertiesItem* sopItem = static_cast<SceneObjectPropertiesItem*>(index.internalPointer());
    switch (sopItem->getProperty())
    {
        case SCENE_OBJECT_PROPERTY_CHILD:
            break;
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION:
            break;

        case SCENE_OBJECT_PROPERTY_TRANSLATION_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_ORIENTATION_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_UUID:
        case SCENE_OBJECT_PROPERTY_NAME:
            static_cast<QLineEdit*>(editor)->setText(value.toString());
            break;

        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_W:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_X:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Y:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Z:
        case SCENE_OBJECT_PROPERTY_SCALE_X:
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            static_cast<QDoubleSpinBox*>(editor)->setValue(value.toDouble());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            static_cast<QComboBox*>(editor)->setCurrentText(value.toString());
            break;
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
        case SCENE_OBJECT_PROPERTY_FOLLOWS_CAMERA:
        case SCENE_OBJECT_PROPERTY_ALWAYS_DRAW:
        case SCENE_OBJECT_PROPERTY_STATIC:
            static_cast<QCheckBox*>(editor)->setChecked(value.toBool());
            break;
        case SCENE_OBJECT_PROPERTY_NONE:
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION_LIST:
        case SCENE_OBJECT_PROPERTY_CHILD_LIST:
            break;

    }
}

void SceneObjectPropertiesTreeDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    SceneObjectPropertiesItem* sopItem = static_cast<SceneObjectPropertiesItem*>(index.internalPointer());
    switch (sopItem->getProperty())
    {
        case SCENE_OBJECT_PROPERTY_UUID:
        case SCENE_OBJECT_PROPERTY_NAME:
            model->setData(index, static_cast<QLineEdit*>(editor)->text());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_W:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_X:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Y:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Z:
        case SCENE_OBJECT_PROPERTY_SCALE_X:
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            model->setData(index, static_cast<QDoubleSpinBox*>(editor)->value());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            model->setData(index,static_cast<QComboBox*>(editor)->currentText());
            break;
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
        case SCENE_OBJECT_PROPERTY_FOLLOWS_CAMERA:
        case SCENE_OBJECT_PROPERTY_ALWAYS_DRAW:
        case SCENE_OBJECT_PROPERTY_STATIC:
            model->setData(index,static_cast<QCheckBox*>(editor)->isChecked());
            break;

        case SCENE_OBJECT_PROPERTY_CHILD:
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_CAPTURE:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_CAPTURE:
        case SCENE_OBJECT_PROPERTY_SCALE_CAPTURE:
        case SCENE_OBJECT_PROPERTY_NONE:
        case SCENE_OBJECT_PROPERTY_CHILD_LIST:
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION_LIST:
            break;
    }
}

void
SceneObjectPropertiesTreeDelegate::updateEditorGeometry
(QWidget *editor,const  QStyleOptionViewItem &option,const  QModelIndex &index)
const
{
    editor->setGeometry(option.rect);
}

void
SceneObjectPropertiesTreeDelegate::onButton_CaptureTranslation
(bool)
{
    auto log = spdlog::get("SceneObjectPropertiesTreeDelegate");
    log->debug("CaptureTranslation");
    emit notifyButton_CaptureTranslation();
}

void
SceneObjectPropertiesTreeDelegate::onButton_CaptureOrientation
(bool)
{
    auto log = spdlog::get("SceneObjectPropertiesTreeDelegate");
    log->debug("CaptureOrientation");
    emit notifyButton_CaptureOrientation();
}

void
SceneObjectPropertiesTreeDelegate::onButton_CaptureScale
(bool)
{
    auto log = spdlog::get("SceneObjectPropertiesTreeDelegate");
    log->debug("CaptureScale");
    emit notifyButton_CaptureScale();
}

void
SceneObjectPropertiesTreeDelegate::onButton_RemoveAsset
(bool, DreamObject* vHandle)
{
    auto log = spdlog::get("SceneObjectPropertiesTreeDelegate");
    log->debug("RemoveAsset");
    IAssetDefinition* adHandle = dynamic_cast<IAssetDefinition*>(vHandle);
    emit notifyButton_RemoveAsset(adHandle);
}

void
SceneObjectPropertiesTreeDelegate::onButton_RemoveChild
(bool, DreamObject* vHandle)
{
    auto log = spdlog::get("SceneObjectPropertiesTreeDelegate");
    log->debug("RemoveChild");
    SceneObjectDefinition* sodHandle = dynamic_cast<SceneObjectDefinition*>(vHandle);
    emit notifyButton_RemoveChild(sodHandle);
}

QWidget*
SceneObjectPropertiesTreeDelegate::createTransformTypeComboBox
(QWidget* parent)
const
{
    QComboBox *combo = new QComboBox(parent);
    combo->addItem(QString::fromStdString(Constants::TRANSFORM_TYPE_ABSOLUTE));
    combo->addItem(QString::fromStdString(Constants::TRANSFORM_TYPE_OFFSET));
    return combo;
}

QWidget*
SceneObjectPropertiesTreeDelegate::createCaptureTranslationButton
(QWidget* parent)
const
{
    QToolButton *button = new QToolButton(parent);
    button->setText("Capture Translation");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_CaptureTranslation(bool))
    );
    return button;
}

QWidget*
SceneObjectPropertiesTreeDelegate::createCaptureOrientationButton
(QWidget* parent)
const
{
    QToolButton *button = new QToolButton(parent);
    button->setText("Capture Orientation");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_CaptureOrientation(bool))
    );
    return button;
}

QWidget*
SceneObjectPropertiesTreeDelegate::createCaptureScaleButton
(QWidget* parent)
const
{
    QToolButton *button = new QToolButton(parent);
    button->setText("Capture Scale");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_CaptureScale(bool))
    );
    return button;
}


QWidget*
SceneObjectPropertiesTreeDelegate::createRemoveAssetDefinitionButton
(IAssetDefinition* adHandle, QWidget *parent)
const
{
    TreeModelToolButton* button = new TreeModelToolButton(adHandle,parent);
    button->setText("Remove Asset");
    connect
    (
        button,
        SIGNAL(notifyClickedWithData(bool,DreamObject*)),
        this,
        SLOT(onButton_RemoveAsset(bool,DreamObject*))
    );
    return button;
}

QWidget*
SceneObjectPropertiesTreeDelegate::createRemoveChildButton
(SceneObjectDefinition* sodHandle, QWidget *parent)
const
{
   TreeModelToolButton* button = new TreeModelToolButton(sodHandle,parent);
   button->setText("Remove Child");
   connect
    (
        button,
        SIGNAL(notifyClickedWithData(bool,DreamObject*)),
        this,
        SLOT(onButton_RemoveChild(bool,DreamObject*))
    );
   return button;
}
