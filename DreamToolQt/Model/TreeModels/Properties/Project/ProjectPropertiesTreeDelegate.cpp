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
ProjectPropertiesTreeDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ProjectPropertiesItem* ppi = static_cast<ProjectPropertiesItem*>(index.internalPointer());
    switch(ppi->getProperty())
    {
        case PROJECT_PROPERTY_NAME:
        case PROJECT_PROPERTY_AUTHOR:
        case PROJECT_PROPERTY_NONE:
            return new QLineEdit(parent);
    }
    return new QLineEdit(parent);
}

void
ProjectPropertiesTreeDelegate::setEditorData
(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    ProjectPropertiesItem* ppi = static_cast<ProjectPropertiesItem*>(index.internalPointer());
    switch(ppi->getProperty())
    {
        case PROJECT_PROPERTY_NAME:
        case PROJECT_PROPERTY_AUTHOR:
        case PROJECT_PROPERTY_NONE:
            static_cast<QLineEdit*>(editor)->setText(value);
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
        case PROJECT_PROPERTY_NAME:
        case PROJECT_PROPERTY_AUTHOR:
        case PROJECT_PROPERTY_NONE:
            model->setData(index,static_cast<QLineEdit*>(editor)->text());
            break;
    }
}

void
ProjectPropertiesTreeDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
