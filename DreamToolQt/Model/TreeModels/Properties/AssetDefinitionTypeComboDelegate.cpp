/*
 * AssetDefinitionTypeComboDelegate.cpp
 *
 * Created: 20 2017 by Ashley
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
#include "AssetDefinitionTypeComboDelegate.h"
#include <DreamCore.h>
#include <QComboBox>

AssetDefinitionTypeComboDelegate::AssetDefinitionTypeComboDelegate
(QObject* parent)
    : QItemDelegate (parent)
{

}

QWidget*
AssetDefinitionTypeComboDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index )
const
{
    QComboBox *editor = new QComboBox(parent);
    QStringList list ;
    for (std::pair<Dream::AssetType,std::string> type : Dream::DREAM_ASSET_TYPES_MAP)
    {
        list << QString::fromStdString(type.second);
    }
    editor->addItems(list);
    return editor;
}

void
AssetDefinitionTypeComboDelegate::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->addItem(value);
}

void
AssetDefinitionTypeComboDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value);
}

void
AssetDefinitionTypeComboDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index )
const
{
    editor->setGeometry(option.rect);
}
