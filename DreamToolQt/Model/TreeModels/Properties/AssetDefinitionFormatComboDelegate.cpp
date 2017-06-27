/*
 * AssetDefinitionFormatComboDelegate.cpp
 *
 * Created: 30 2017 by Ashley
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
#include "AssetDefinitionFormatComboDelegate.h"
#include "../AssetDefinitionTreeItem.h"
#include <QComboBox>
#include <QDebug>

using Dream::Constants;

AssetDefinitionFormatComboDelegate::AssetDefinitionFormatComboDelegate(std::string type, QObject* parent)
    : QItemDelegate(parent)
{
   qDebug() << "AssetDefinitionFormatComboDelegate: Constructing Object";
    mAssetType = Constants::assetTypeFromString(type);
}

AssetDefinitionFormatComboDelegate::~AssetDefinitionFormatComboDelegate
()
{
   qDebug() << "AssetDefinitionFormatComboDelegate: Destroying Object";
}

QWidget*
AssetDefinitionFormatComboDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index )
const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setDuplicatesEnabled(false);
    editor->setEditable(false);
    editor->setInsertPolicy(QComboBox::NoInsert);
    vector<std::string> formats = Constants::DREAM_ASSET_FORMATS_MAP[mAssetType];
    QStringList list;
    for (std::string format : formats)
    {
        list.push_back(QString::fromStdString(format));
    }
    editor->addItems(list);
    return editor;
}

void
AssetDefinitionFormatComboDelegate::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(value);
}

void
AssetDefinitionFormatComboDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value);
}

void
AssetDefinitionFormatComboDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index )
const
{
    editor->setGeometry(option.rect);
}
