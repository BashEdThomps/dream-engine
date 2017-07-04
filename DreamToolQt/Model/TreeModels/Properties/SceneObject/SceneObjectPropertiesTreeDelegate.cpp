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
#include <QDebug>
#include <QItemDelegate>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QToolButton>

#include "SceneObjectPropertiesModel.h"
#include "SceneObjectPropertiesItem.h"

using Dream::Constants;

SceneObjectPropertiesTreeDelegate::SceneObjectPropertiesTreeDelegate
(SceneObjectPropertiesModel* model, QObject* parent)
    : QItemDelegate (parent),
      mModelHandle(model)

{
    qDebug() << "SceneObjectPropertiesTreeDelegate: Constructing";
}

SceneObjectPropertiesTreeDelegate::~SceneObjectPropertiesTreeDelegate
()
{
    qDebug() << "SceneObjectPropertiesTreeDelegate: Destructing";
}

QWidget*
SceneObjectPropertiesTreeDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    SceneObjectPropertiesItem* sopItem = static_cast<SceneObjectPropertiesItem*>(index.internalPointer());

    switch (sopItem->getProperty())
    {
        case SCENE_OBJECT_PROPERTY_CHILD:
            return createRemoveChildButton(parent);
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION:
            return createRemoveAssetDefinitionButton(parent);
        case SCENE_OBJECT_PROPERTY_TRANSLATION_CAPTURE:
            return createCaptureTranslationButton(parent);
        case SCENE_OBJECT_PROPERTY_ROTATION_CAPTURE:
            return createCaptureRotationButton(parent);
        case SCENE_OBJECT_PROPERTY_SCALE_CAPTURE:
            return createCaptureScaleButton(parent);
        case SCENE_OBJECT_PROPERTY_NAME:
            return new QLineEdit(parent);
        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
        case SCENE_OBJECT_PROPERTY_ROTATION_X:
        case SCENE_OBJECT_PROPERTY_ROTATION_Y:
        case SCENE_OBJECT_PROPERTY_ROTATION_Z:
        case SCENE_OBJECT_PROPERTY_SCALE_X:
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            return new QDoubleSpinBox(parent);
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            return createTransformTypeComboBox(parent);
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
            return new QCheckBox(parent);
        case SCENE_OBJECT_PROPERTY_NONE:
            return new QLineEdit(parent);

    }

    return new QLineEdit(parent);
}

void
SceneObjectPropertiesTreeDelegate::setEditorData
(QWidget *editor, const QModelIndex &index) const
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
        case SCENE_OBJECT_PROPERTY_ROTATION_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_NAME:
            static_cast<QLineEdit*>(editor)->setText(value.toString());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
        case SCENE_OBJECT_PROPERTY_ROTATION_X:
        case SCENE_OBJECT_PROPERTY_ROTATION_Y:
        case SCENE_OBJECT_PROPERTY_ROTATION_Z:
        case SCENE_OBJECT_PROPERTY_SCALE_X:
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            static_cast<QDoubleSpinBox*>(editor)->setValue(value.toDouble());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            static_cast<QComboBox*>(editor)->setCurrentText(value.toString());
            break;
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
            static_cast<QCheckBox*>(editor)->setChecked(value.toBool());
            break;
        case SCENE_OBJECT_PROPERTY_NONE:
            break;

    }
}

void SceneObjectPropertiesTreeDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    SceneObjectPropertiesItem* sopItem = static_cast<SceneObjectPropertiesItem*>(index.internalPointer());
    switch (sopItem->getProperty())
    {
        case SCENE_OBJECT_PROPERTY_CHILD:
            break;
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION:
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_ROTATION_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_NAME:
            model->setData(index, static_cast<QLineEdit*>(editor)->text());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
        case SCENE_OBJECT_PROPERTY_ROTATION_X:
        case SCENE_OBJECT_PROPERTY_ROTATION_Y:
        case SCENE_OBJECT_PROPERTY_ROTATION_Z:
        case SCENE_OBJECT_PROPERTY_SCALE_X:
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            model->setData(index, static_cast<QDoubleSpinBox*>(editor)->value());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            model->setData(index,static_cast<QComboBox*>(editor)->currentText());
            break;
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
            model->setData(index,static_cast<QCheckBox*>(editor)->isChecked());
            break;
        case SCENE_OBJECT_PROPERTY_NONE:
            break;

    }

}

void
SceneObjectPropertiesTreeDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void
SceneObjectPropertiesTreeDelegate::onButton_CaptureTranslation
(bool)
{
    qDebug() << "SceneObjectPropertiesDelegate: CaptureTranslation";
    emit notifyButton_CaptureTranslation();
}

void
SceneObjectPropertiesTreeDelegate::onButton_CaptureRotation
(bool)
{
    qDebug() << "SceneObjectPropertiesDelegate: CaptureRotation";
    emit notifyButton_CaptureRotation();
}

void
SceneObjectPropertiesTreeDelegate::onButton_CaptureScale
(bool)
{
    qDebug() << "SceneObjectPropertiesDelegate: CaptureScale";
    emit notifyButton_CaptureScale();
}

void
SceneObjectPropertiesTreeDelegate::onButton_RemoveAsset
(bool)
{
    qDebug() << "SceneObjectPropertiesDelegate: RemoveAsset";
    emit notifyButton_RemoveAsset();
}

void
SceneObjectPropertiesTreeDelegate::onButton_RemoveChild
(bool)
{
    qDebug() << "SceneObjectPropertiesDelegate: RemoveChild";
    emit notifyButton_RemoveChild();
}

QWidget*
SceneObjectPropertiesTreeDelegate::createTransformTypeComboBox
(QWidget* parent) const
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
SceneObjectPropertiesTreeDelegate::createCaptureRotationButton
(QWidget* parent)
const
{
    QToolButton *button = new QToolButton(parent);
    button->setText("Capture Rotation");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_CaptureRotation(bool))
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
(QWidget *parent)
const
{
    QToolButton* button = new QToolButton(parent);
    button->setText("Remove Asset");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_RemoveAsset(bool))
    );
    return button;
}

QWidget*
SceneObjectPropertiesTreeDelegate::createRemoveChildButton
(QWidget *parent)
const
{
   QToolButton* button = new QToolButton(parent);
   button->setText("Remove Child");
   connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_RemoveChild(bool))
    );
   return button;
}
