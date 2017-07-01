/*
 * DoubleSpinBoxDelegate.cpp
 *
 * Created: 28 2017 by Ashley
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

#include "DoubleSpinBoxDelegate.h"
#include <QDoubleSpinBox>

#include <QDebug>

using Dream::Constants;

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate
(QObject* parent)
    : QItemDelegate(parent)
{
   qDebug() << "AbstractDoubleSpinBoxDelegate: Constructing Object";
}

DoubleSpinBoxDelegate::~DoubleSpinBoxDelegate
()
{
   qDebug() << "AbstractDoubleSpinBoxDelegate: Destroying Object";
}

QWidget*
DoubleSpinBoxDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index )
const
{
    qDebug() << "AbstractDoubleSpinBoxDelegate: Creating spinbox";
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    return editor;
}

void
DoubleSpinBoxDelegate::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    QString value = index.model()->data(index).toString();
    qDebug() << "DoubleSpinBoxDelegate: setEditorData to" << value << value.toDouble();
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value.toDouble());
}

void
DoubleSpinBoxDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    double value = spinBox->value();
    model->setData(index, QString::number(value));
}

void
DoubleSpinBoxDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index )
const
{
    editor->setGeometry(option.rect);
}
