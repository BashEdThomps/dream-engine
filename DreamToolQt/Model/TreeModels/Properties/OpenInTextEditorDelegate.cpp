/*
 * OpenInTextEditorDelegate.cpp
 *
 * Created: 27 2017 by Ashley
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
#include "OpenInTextEditorDelegate.h"


#include <QDebug>
#include <QToolButton>
#include <DreamCore.h>

OpenInTextEditorDelegate::OpenInTextEditorDelegate
(AssetDefinition* adHandle, QObject* parent)
    : QItemDelegate (parent),
      mAssetDefinitionHandle(adHandle)
{
    qDebug() << "OpenInTextEditorDelegate: Constructing";
}

OpenInTextEditorDelegate::~OpenInTextEditorDelegate
()
{
    qDebug() << "OpenInTextEditorDelegate: Destructing";
}

QWidget*
OpenInTextEditorDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index )
const
{
    QToolButton *editor = new QToolButton(parent);
    editor->setText("Open in Text Editor");
    connect
    (
        editor,SIGNAL(clicked(bool)),
        this, SLOT(onOpenButtonClicked(bool))
    );
    return editor;
}

void
OpenInTextEditorDelegate::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    /*
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->addItem(value);
    */
}

void
OpenInTextEditorDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    /*
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString value = comboBox->currentText();
        model->setData(index, value);
    */
}

void
OpenInTextEditorDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index )
const
{
    editor->setGeometry(option.rect);
}

void
OpenInTextEditorDelegate::onOpenButtonClicked
(bool checked)
{
    qDebug() << "OpenInTextEditorDelegate: Open was clicked"
             << checked;

    emit notifyOpenInTextEditorButtonClicked(mAssetDefinitionHandle);
}
