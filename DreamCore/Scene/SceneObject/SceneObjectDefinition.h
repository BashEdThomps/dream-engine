/*
 * SceneObjectDefinition.h
 *
 * Created: 16 2017 by Ashley
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

#include <iostream>
#include <vector>
#include <memory>

#include "../../Common/ILoggable.h"
#include "../../Common/IDefinition.h"
#include "../../Components/Transform3D.h"

using std::string;
using std::vector;
using std::unique_ptr;

namespace Dream
{
    class IAssetDefinition;
    class SceneDefinition;

    class SceneObjectDefinition : public IDefinition, ILoggable
    {
    private:
        SceneObjectDefinition* mParentSceneObjectHandle;
        SceneDefinition* mSceneDefinitionHandle;
        vector<unique_ptr<SceneObjectDefinition>> mChildDefinitions;
        Transform3D mTransform;

    public:
        SceneObjectDefinition(SceneObjectDefinition* parentHandle, SceneDefinition* sceneDefinitionHandle,  json data);
        ~SceneObjectDefinition();

        void setHasFocus(bool focus);
        bool hasFocus();

        void setFollowsCamera(bool fc);
        bool followsCamera();

        void addAssetDefinitionToLoadQueue(IAssetDefinition* adHandle);
        void addAssetDefinitionUuidToLoadQueue(string uuid);

        void removeAssetDefinitionFromLoadQueue(IAssetDefinition* adHandle);
        void removeAssetDefinitionUuidFromLoadQueue(string uuid);

        vector<string> getAssetDefinitionLoadQueue();

        Transform3D& getTransform();

        void showStatus() override;

        vector<SceneObjectDefinition*> getChildDefinitionsHandleList();
        void addChildSceneObjectDefinition(SceneObjectDefinition* child);
        void removeChildSceneObjectDefinition(SceneObjectDefinition* child);
        SceneObjectDefinition* createNewChildSceneObjectDefinition();

        SceneDefinition *getSceneDefinitionHandle();
        json getJson() override;

    private:
        void loadChildSceneObjectDefinitions(json definition);
    };

}
