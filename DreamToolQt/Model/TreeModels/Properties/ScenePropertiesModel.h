/*
 * ScenePropertiesModel.h
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

#ifndef SCENEPROPERTIESMODEL_H
#define SCENEPROPERTIESMODEL_H

#include "PropertiesModel.h"
#include <DreamCore.h>

using Dream::SceneDefinition;

class ScenePropertiesModel : public PropertiesModel
{
public:
    ScenePropertiesModel(SceneDefinition *scene, QTreeView* parent = 0);
    ~ScenePropertiesModel();

    void createRoot() override;
    void createProperties() override;
    void createDelegates() override;

    void createNameProperties();
    void createNotesProperties();
    void createCameraProperties();
    void createRenderingProperties();
    void createPhysicsProperties();

private:
    SceneDefinition *mScene;
};

#endif // SCENEPROPERTIESMODEL_H
