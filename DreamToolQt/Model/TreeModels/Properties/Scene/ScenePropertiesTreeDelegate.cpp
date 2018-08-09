/*
 * ScenePropertiesTreeDelegate.cpp
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
#include "ScenePropertiesTreeDelegate.h"

#include "ScenePropertiesModel.h"
#include "ScenePropertiesItem.h"

#include <spdlog/spdlog.h>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QToolButton>

using std::numeric_limits;

ScenePropertiesTreeDelegate::ScenePropertiesTreeDelegate
(ScenePropertiesModel* model, QObject* parent)
    : QItemDelegate(parent),
      mModelHandle(model)
{
    auto log = spdlog::get("ScenePropertiesTreeDelegate");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("ScenePropertiesTreeDelegate");
    }
    log->info("Constructing");
    setClipping(true);
}

ScenePropertiesTreeDelegate::~ScenePropertiesTreeDelegate
()
{
    auto log = spdlog::get("ScenePropertiesTreeDelegate");
    log->info("Constructing");
}

QWidget*
ScenePropertiesTreeDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex &index) const
{
    ScenePropertiesItem* spiHandle = static_cast<ScenePropertiesItem*>(index.internalPointer());
    QDoubleSpinBox *spinbox = nullptr;
    SceneProperty property = spiHandle->getProperty();
    switch (property)
    {
        case SCENE_PROPERTY_CAMERA:
            return createCameraAllCaptureButton(parent);
        case SCENE_PROPERTY_CAMERA_TRANSLATION_CAPTURE:
            return createCameraTranslationCaptureButton(parent);
        case SCENE_PROPERTY_CAMERA_ROTATION_CAPTURE:
            return createCameraRotationCaptureButton(parent);
        case SCENE_PROPERTY_NAME:
        case SCENE_PROPERTY_NOTES:
            return new QLineEdit(parent);

        case SCENE_PROPERTY_CAMERA_TRANSLATION_X:
        case SCENE_PROPERTY_CAMERA_TRANSLATION_Y:
        case SCENE_PROPERTY_CAMERA_TRANSLATION_Z:

        case SCENE_PROPERTY_CAMERA_ROTATION_X:
        case SCENE_PROPERTY_CAMERA_ROTATION_Y:
        case SCENE_PROPERTY_CAMERA_ROTATION_Z:

        case SCENE_PROPERTY_CAMERA_SPEED:

        case SCENE_PROPERTY_CLEAR_RED:
        case SCENE_PROPERTY_CLEAR_GREEN:
        case SCENE_PROPERTY_CLEAR_BLUE:

        case SCENE_PROPERTY_AMBIENT_RED:
        case SCENE_PROPERTY_AMBIENT_GREEN:
        case SCENE_PROPERTY_AMBIENT_BLUE:
        case SCENE_PROPERTY_AMBIENT_ALPHA:

        case SCENE_PROPERTY_PHYSICS_GRAVITY_X:
        case SCENE_PROPERTY_PHYSICS_GRAVITY_Y:
        case SCENE_PROPERTY_PHYSICS_GRAVITY_Z:
            spinbox = new QDoubleSpinBox(parent);
            spinbox->setRange(numeric_limits<double>::lowest(), numeric_limits<double>::max());
            spinbox->setDecimals(3);
            return spinbox;
        case SCENE_PROPERTY_PHYSICS_DEBUG:
            return new QCheckBox(parent);

        case SCENE_PROPERTY_AMBIENT_PARENT:
           return createAmbientColourPaletteButton(parent);

        case SCENE_PROPERTY_CLEAR_PARENT:
           return createClearColourPaletteButton(parent);

        case SCENE_PROPERTY_NONE:
            return new QLabel(parent);
    }
}

void
ScenePropertiesTreeDelegate::setEditorData
(QWidget *editor, const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::DisplayRole);

    ScenePropertiesItem* spiHandle = static_cast<ScenePropertiesItem*>(index.internalPointer());
    switch (spiHandle->getProperty())
    {
        case SCENE_PROPERTY_CAMERA:
            break;
        case SCENE_PROPERTY_CAMERA_TRANSLATION_CAPTURE:
            break;
        case SCENE_PROPERTY_CLEAR_PARENT:
        case SCENE_PROPERTY_AMBIENT_PARENT:
        case SCENE_PROPERTY_CAMERA_ROTATION_CAPTURE:
            break;
        case SCENE_PROPERTY_NAME:
        case SCENE_PROPERTY_NOTES:
            static_cast<QLineEdit*>(editor)->setText(value.toString());
            break;

        case SCENE_PROPERTY_CAMERA_TRANSLATION_X:
        case SCENE_PROPERTY_CAMERA_TRANSLATION_Y:
        case SCENE_PROPERTY_CAMERA_TRANSLATION_Z:

        case SCENE_PROPERTY_CAMERA_ROTATION_X:
        case SCENE_PROPERTY_CAMERA_ROTATION_Y:
        case SCENE_PROPERTY_CAMERA_ROTATION_Z:

        case SCENE_PROPERTY_CAMERA_SPEED:

        case SCENE_PROPERTY_CLEAR_RED:
        case SCENE_PROPERTY_CLEAR_GREEN:
        case SCENE_PROPERTY_CLEAR_BLUE:

        case SCENE_PROPERTY_AMBIENT_RED:
        case SCENE_PROPERTY_AMBIENT_GREEN:
        case SCENE_PROPERTY_AMBIENT_BLUE:
        case SCENE_PROPERTY_AMBIENT_ALPHA:

        case SCENE_PROPERTY_PHYSICS_GRAVITY_X:
        case SCENE_PROPERTY_PHYSICS_GRAVITY_Y:
        case SCENE_PROPERTY_PHYSICS_GRAVITY_Z:
            static_cast<QDoubleSpinBox*>(editor)->setValue(value.toDouble());
            break;

        case SCENE_PROPERTY_PHYSICS_DEBUG:
            static_cast<QCheckBox*>(editor)->setChecked(value.toBool());
            break;

        case SCENE_PROPERTY_NONE:
            break;

    }
}

void
ScenePropertiesTreeDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    ScenePropertiesItem* spiHandle = static_cast<ScenePropertiesItem*>(index.internalPointer());
    switch (spiHandle->getProperty())
    {
        case SCENE_PROPERTY_CAMERA:
            break;
        case SCENE_PROPERTY_CAMERA_TRANSLATION_CAPTURE:
            break;
        case SCENE_PROPERTY_CAMERA_ROTATION_CAPTURE:
            break;
        case SCENE_PROPERTY_NAME:
        case SCENE_PROPERTY_NOTES:
            model->setData(index, static_cast<QLineEdit*>(editor)->text());
            break;

        case SCENE_PROPERTY_CAMERA_TRANSLATION_X:
        case SCENE_PROPERTY_CAMERA_TRANSLATION_Y:
        case SCENE_PROPERTY_CAMERA_TRANSLATION_Z:

        case SCENE_PROPERTY_CAMERA_ROTATION_X:
        case SCENE_PROPERTY_CAMERA_ROTATION_Y:
        case SCENE_PROPERTY_CAMERA_ROTATION_Z:

        case SCENE_PROPERTY_CAMERA_SPEED:

        case SCENE_PROPERTY_CLEAR_RED:
        case SCENE_PROPERTY_CLEAR_GREEN:
        case SCENE_PROPERTY_CLEAR_BLUE:

        case SCENE_PROPERTY_AMBIENT_RED:
        case SCENE_PROPERTY_AMBIENT_GREEN:
        case SCENE_PROPERTY_AMBIENT_BLUE:
        case SCENE_PROPERTY_AMBIENT_ALPHA:

        case SCENE_PROPERTY_PHYSICS_GRAVITY_X:
        case SCENE_PROPERTY_PHYSICS_GRAVITY_Y:
        case SCENE_PROPERTY_PHYSICS_GRAVITY_Z:
            model->setData(index,static_cast<QDoubleSpinBox*>(editor)->value());
            break;

        case SCENE_PROPERTY_PHYSICS_DEBUG:
            model->setData(index,static_cast<QCheckBox*>(editor)->isChecked());
            break;

        case SCENE_PROPERTY_NONE:
            model->setData(index, static_cast<QLabel*>(editor)->text());
            break;

        case SCENE_PROPERTY_AMBIENT_PARENT:
        case SCENE_PROPERTY_CLEAR_PARENT:
            break;
    }
}

void
ScenePropertiesTreeDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

void
ScenePropertiesTreeDelegate::onButton_CaptureCameraTranslation
(bool)
{
    auto log = spdlog::get("ScenePropertiesTreeDelegate");
    log->info("CaptureCameraTranslation");
    emit notifyButton_CaptureCameraTranslation();
}

void
ScenePropertiesTreeDelegate::onButton_CaptureCameraRotation
(bool)
{
    auto log = spdlog::get("ScenePropertiesTreeDelegate");
    log->info("CaptureCameraRotation");
    emit notifyButton_CaptureCameraRotation();
}

void ScenePropertiesTreeDelegate::onButton_CaptureCameraAll(bool)
{
    auto log = spdlog::get("ScenePropertiesTreeDelegate");
    log->info("CaptureCameraAll");
    emit notifyButton_CaptureCameraTranslation();
    emit notifyButton_CaptureCameraRotation();
}

void ScenePropertiesTreeDelegate::onButton_ChooseAmbientColour(bool)
{
    auto log = spdlog::get("ScenePropertiesTreeDelegate");
    log->info("ChooseAmbientColour");
    emit notifyButton_ChooseAmbientColour();
}

void ScenePropertiesTreeDelegate::onButton_ChooseClearColour(bool)
{
    auto log = spdlog::get("ScenePropertiesTreeDelegate");

    log->info("ChooseClearColour");
    emit notifyButton_ChooseClearColour();
}

QWidget*
ScenePropertiesTreeDelegate::createCameraAllCaptureButton
(QWidget* parent) const
{
    QToolButton* button = new QToolButton(parent);
    button->setText("Capture");

    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_CaptureCameraAll(bool))
    );

    return button;
}

QWidget*
ScenePropertiesTreeDelegate::createCameraTranslationCaptureButton
(QWidget* parent) const
{
    QToolButton* button = new QToolButton(parent);
    button->setText("Capture Translation");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_CaptureCameraTranslation(bool))
    );
    return button;
}

QWidget*
ScenePropertiesTreeDelegate::createCameraRotationCaptureButton
(QWidget* parent) const
{
    QToolButton* button = new QToolButton(parent);
    button->setText("Capture Rotation");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_CaptureCameraRotation(bool))
    );
    return button;
}

QWidget*ScenePropertiesTreeDelegate::createAmbientColourPaletteButton(QWidget* parent) const
{
    QToolButton* button = new QToolButton(parent);
    button->setText("Choose Colour...");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_ChooseAmbientColour(bool))
    );
    return button;
}

QWidget*ScenePropertiesTreeDelegate::createClearColourPaletteButton(QWidget* parent) const
{
    QToolButton* button = new QToolButton(parent);
    button->setText("Choose Colour...");
    connect
    (
        button,
        SIGNAL(clicked(bool)),
        this,
        SLOT(onButton_ChooseClearColour(bool))
    );
    return button;
}