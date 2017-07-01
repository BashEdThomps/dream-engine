/*
 * ScenePropertiesItem.cpp
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
#include "ScenePropertiesItem.h"

ScenePropertiesItem::ScenePropertiesItem
(
    QString title,
    SceneDefinition* sdHandle,
    SceneProperty property,
    QItemDelegate* delegate,
    AbstractPropertiesItem *parent
)
    : AbstractPropertiesItem(title,delegate,parent),
      mSceneDefinitionHandle(sdHandle),
      mProperty(property)
{

}

ScenePropertiesItem::~ScenePropertiesItem
()
{

}

QVariant ScenePropertiesItem::data(int column)
{
    return QVariant();
}

bool ScenePropertiesItem::setData(int column, const QVariant &value)
{
    return false;
}
