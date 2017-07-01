/*
 * CheckBoxDelegate.cpp
 *
 * Created: 29 2017 by Ashley
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
#include "CheckBoxDelegate.h"

#include <QDebug>
#include <QCheckBox>

CheckBoxDelegate::CheckBoxDelegate(QObject* parent)
    : QItemDelegate (parent)
{
    qDebug() << "CheckBoxDelegate: Constructing";
}

CheckBoxDelegate::~CheckBoxDelegate()
{
    qDebug() << "CheckBoxDelegate: Destructing";
}

QWidget*
CheckBoxDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index )
const
{
    qDebug() << "CheckBoxDelegate: Creating checkbox";
    QCheckBox *editor = new QCheckBox(parent);
    return editor;
}

void
CheckBoxDelegate::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    bool value = index.model()->data(index, Qt::DisplayRole).toBool();
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    checkBox->setCheckState(value ? Qt::Checked : Qt::Unchecked);
}

void
CheckBoxDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    Qt::CheckState state = checkBox->checkState();
    model->setData(index, state);
}

void
CheckBoxDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index )
const
{
    editor->setGeometry(option.rect);
}
