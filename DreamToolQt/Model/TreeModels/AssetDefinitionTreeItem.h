/*
 * AssetDefinitionTreeItem.h
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

#pragma once

#include <DreamCore.h>
#include "GenericTreeItem.h"
#include <QList>
#include <QVariant>

class AssetDefinitionTreeItem : public GenericTreeItem
{
public:

    explicit AssetDefinitionTreeItem(
        const QList<QVariant> &data,
        GenericTreeItemType type,
        Dream::AssetDefinition* definiion,
        AssetDefinitionTreeItem *parentItem = 0
    );

    Dream::AssetDefinition *getAssetDefinition();

private:
    Dream::AssetDefinition *mDefinition;
};
