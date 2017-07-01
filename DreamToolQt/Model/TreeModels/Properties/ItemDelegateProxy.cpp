/*
 * ItemDelegateProxy.cpp
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

#include "ItemDelegateProxy.h"
#include "AbstractPropertiesItem.h"

#include <QDebug>
#include <QLineEdit>

ItemDelegateProxy::ItemDelegateProxy
(QObject* parent)
    : QItemDelegate (parent)
{
    qDebug() << "ItemDelegateProxy: Constructing";
}

ItemDelegateProxy::~ItemDelegateProxy
()
{
    qDebug() << "ItemDelegateProxy: Destructing";
}

QWidget*
ItemDelegateProxy::createEditor
(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)
const
{
    QItemDelegate* pi = static_cast<AbstractPropertiesItem*>(index.internalPointer())->getDelegate();
    if (pi)
    {
        qDebug() << "ItemDelegateProxy: createEditor got delegate for"
                 << index.column() << "," << index.row();
        return pi->createEditor(parent,option,index);
    }
    qDebug() << "ItemDelegateProxy: createEditor getting delegate for"
             << index.column() << "," << index.row();
    return new QLineEdit(parent);
}

void
ItemDelegateProxy::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    QItemDelegate* pi = static_cast<AbstractPropertiesItem*>(index.internalPointer())->getDelegate();
    if (pi)
    {
        qDebug() << "ItemDelegateProxy: setEditorData got delegate for"
                 << index.column() << ","  << index.row();
        pi->setEditorData(editor,index);
        return;
    }
    qDebug() << "ItemDelegateProxy: setEditorData no delegate for"
             << index.column() << ","  << index.row();
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void
ItemDelegateProxy::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    QItemDelegate* pi = static_cast<AbstractPropertiesItem*>(index.internalPointer())->getDelegate();
    if (pi)
    {
        qDebug() << "ItemDelegateProxy: setModelData got delegate for"
                 << index.column() << ","  << index.row();
        pi->setModelData(editor,model,index);
        return;
    }
    qDebug() << "ItemDelegateProxy: setModelData no delegate for"
             << index.column() << ","  << index.row();
    QLineEdit *comboBox = static_cast<QLineEdit*>(editor);
    QString value = comboBox->text();
    model->setData(index, value);
}

void
ItemDelegateProxy::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)
const
{
    QItemDelegate* pi = static_cast<AbstractPropertiesItem*>(index.internalPointer())->getDelegate();
    if (pi)
    {
        qDebug() << "ItemDelegateProxy: updateEditorGeometry got delegate for"
                 << index.column() << ","  << index.row();
        pi->updateEditorGeometry(editor,option,index);
        return;
    }
    qDebug() << "ItemDelegateProxy: updateEditorGeometry no delegate for"
             << index.column() << ","  << index.row();
    editor->setGeometry(option.rect);
}


