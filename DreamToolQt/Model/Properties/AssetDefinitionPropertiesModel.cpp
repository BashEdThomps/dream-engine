/*
 * AssetDefinitionPropertiesModel.cpp
 *
 * Created: 17 2017 by Ashley
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

#include "AssetDefinitionPropertiesModel.h"
#include <QDebug>

AssetDefinitionPropertiesModel
::AssetDefinitionPropertiesModel(Dream::AssetDefinition *definition, QObject* parent)
    : PropertiesModel(parent)
{
    mAssetDefinition = definition;
    createRoot();
    createProperties();

}

void AssetDefinitionPropertiesModel
::createRoot()
{
    QList<QVariant> rootData;
    rootData << QString::fromStdString(mAssetDefinition->getName())
             << QString::fromStdString(mAssetDefinition->getUuid());
    mRootItem = new PropertiesItem(rootData,mAssetDefinition);
}

void AssetDefinitionPropertiesModel
::createProperties()
{
    // Name
    QList<QVariant> nameData;
    nameData << "Name"
             << QString::fromStdString(mAssetDefinition->getName());

    PropertiesItem *nameProperty = new PropertiesItem(nameData,mAssetDefinition);
    mRootItem->appendChild(nameProperty);
    // Type
    QList<QVariant> typeData;
    typeData << "Type"
             << QString::fromStdString(mAssetDefinition->getType());

    PropertiesItem *typeProperty = new PropertiesItem(typeData,mAssetDefinition);
    mRootItem->appendChild(typeProperty);
    // Format
    QList<QVariant> formatData;
    formatData << "Format"
               << QString::fromStdString(mAssetDefinition->getFormat());
    PropertiesItem *formatProperty = new PropertiesItem(formatData,mAssetDefinition);
    mRootItem->appendChild(formatProperty);

}

AssetDefinitionPropertiesModel
::~AssetDefinitionPropertiesModel()
{
    delete mRootItem;
}





