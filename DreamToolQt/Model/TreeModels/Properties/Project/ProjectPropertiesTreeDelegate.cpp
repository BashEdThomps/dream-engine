/*
 * ProjectPropertiesTreeDelegate.cpp
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
#include "ProjectPropertiesTreeDelegate.h"

#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

#include "ProjectPropertiesModel.h"
#include "ProjectPropertiesItem.h"

ProjectPropertiesTreeDelegate::ProjectPropertiesTreeDelegate
(ProjectPropertiesModel* model, QObject* parent)
    : QItemDelegate(parent),
      mModelHandle(model)
{
    qDebug() << "ProjectPropertiesTreeDelegate: Constructing";
}

ProjectPropertiesTreeDelegate::~ProjectPropertiesTreeDelegate
()
{
    qDebug() << "ProjectPropertiesTreeDelegate: Destructing";
}

QWidget*
ProjectPropertiesTreeDelegate::createStartupSceneComboBox
(ProjectPropertiesItem* ppiItem, QWidget* parent)
const
{
    QComboBox* retval = new QComboBox(parent);
    shared_ptr<ProjectDefinition> pdHandle =  ppiItem->getProjectDefinition();
    QStringList sceneList;
    for (shared_ptr<SceneDefinition> sdHandle : pdHandle->getSceneDefinitionsList())
    {
        sceneList << QString::fromStdString(sdHandle->getName());
    }
    retval->addItems(sceneList);
    return retval;
}

QWidget*
ProjectPropertiesTreeDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex &index) const
{
    ProjectPropertiesItem* ppi = static_cast<ProjectPropertiesItem*>(index.internalPointer());

    switch(ppi->getProperty())
    {
        case PROJECT_PROPERTY_WINDOW_WIDTH:
        case PROJECT_PROPERTY_WINDOW_HEIGHT:
            return new QSpinBox(parent);

        case PROJECT_PROPERTY_STARTUP_SCENE:
            return createStartupSceneComboBox(ppi,parent);

        case PROJECT_PROPERTY_WINDOW_SIZE:
        case PROJECT_PROPERTY_DESCRIPTION:
        case PROJECT_PROPERTY_NAME:
        case PROJECT_PROPERTY_AUTHOR:
        case PROJECT_PROPERTY_NONE:
            return new QLineEdit(parent);

        case PROJECT_PROPERTY_CAPTURE_JOYSTICK:
        case PROJECT_PROPERTY_CAPTURE_KEYBOARD:
        case PROJECT_PROPERTY_CAPTURE_MOUSE:
            return new QCheckBox(parent);
    }
    return new QLineEdit(parent);
}

void
ProjectPropertiesTreeDelegate::setEditorData
(QWidget *editor, const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    ProjectPropertiesItem* ppi = static_cast<ProjectPropertiesItem*>(index.internalPointer());

    switch(ppi->getProperty())
    {
        case PROJECT_PROPERTY_WINDOW_WIDTH:
        case PROJECT_PROPERTY_WINDOW_HEIGHT:
            static_cast<QSpinBox*>(editor)->setValue(value.toInt());
            break;

        case PROJECT_PROPERTY_STARTUP_SCENE:
            static_cast<QComboBox*>(editor)->setCurrentText(value.toString());
            break;

        case PROJECT_PROPERTY_DESCRIPTION:
        case PROJECT_PROPERTY_NAME:
        case PROJECT_PROPERTY_AUTHOR:
        case PROJECT_PROPERTY_NONE:
            static_cast<QLineEdit*>(editor)->setText(value.toString());
            break;
        case PROJECT_PROPERTY_WINDOW_SIZE:
            break;

        case PROJECT_PROPERTY_CAPTURE_KEYBOARD:
        case PROJECT_PROPERTY_CAPTURE_MOUSE:
        case PROJECT_PROPERTY_CAPTURE_JOYSTICK:
            static_cast<QCheckBox*>(editor)->setChecked(value.toBool());
            break;
    }
}

void
ProjectPropertiesTreeDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    ProjectPropertiesItem* ppi = static_cast<ProjectPropertiesItem*>(index.internalPointer());
    switch(ppi->getProperty())
    {
        case PROJECT_PROPERTY_WINDOW_WIDTH:
        case PROJECT_PROPERTY_WINDOW_HEIGHT:
            model->setData(index,static_cast<QSpinBox*>(editor)->value());
            break;

        case PROJECT_PROPERTY_STARTUP_SCENE:
            model->setData(index,static_cast<QComboBox*>(editor)->currentText());
            break;

        case PROJECT_PROPERTY_DESCRIPTION:
        case PROJECT_PROPERTY_NAME:
        case PROJECT_PROPERTY_AUTHOR:
        case PROJECT_PROPERTY_NONE:
            model->setData(index,static_cast<QLineEdit*>(editor)->text());
            break;
        case PROJECT_PROPERTY_WINDOW_SIZE:
            break;

        case PROJECT_PROPERTY_CAPTURE_KEYBOARD:
        case PROJECT_PROPERTY_CAPTURE_MOUSE:
        case PROJECT_PROPERTY_CAPTURE_JOYSTICK:
            model->setData(index,static_cast<QCheckBox*>(editor)->isChecked());
            break;
    }
}

void
ProjectPropertiesTreeDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}
