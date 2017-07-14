/*
 * AssetDefinitionAssetDefinitionTreeItem.cpp
 *
 * Created: 15 2017 by Ashley
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
#include "AssetDefinitionTreeItem.h"
#include <QStringList>
#include <QDebug>
#include <DreamCore.h>

AssetDefinitionTreeItem::AssetDefinitionTreeItem
(
        QString title,
        AssetDefinitionTreeItemType type,
        IAssetDefinition* definition,
        AssetDefinitionTreeItem *parent
)
    : GenericTreeItem(title, parent),
      mDefinition(definition),
      mType(type)
{
    //qDebug() << "AssetDefinitionTreeItem: Constructing";
}

IAssetDefinition* AssetDefinitionTreeItem::getAssetDefinition()
{
   return mDefinition;
}

AssetDefinitionTreeItemType
AssetDefinitionTreeItem::getType
()
const
{
    return mType;
}

void
AssetDefinitionTreeItem::setType
(AssetDefinitionTreeItemType type)
{
    mType = type;
}

QVariant
AssetDefinitionTreeItem::data
(int column)
const
{
    switch (getType())
    {
        case ASSET_DEFINITION:
            return QVariant(QString::fromStdString(mDefinition->getName()));
        case ASSET_TREE_NODE:
            return QVariant(mTitle);
    }
}
