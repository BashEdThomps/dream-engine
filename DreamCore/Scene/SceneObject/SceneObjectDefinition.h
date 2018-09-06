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

#include "../../Common/IDefinition.h"
#include "../../Components/Transform3D.h"

using std::string;
using std::vector;
using std::shared_ptr;

namespace Dream
{
    class IAssetDefinition;
    class SceneDefinition;

    class SceneObjectDefinition : public IDefinition
    {
    private:
        shared_ptr<SceneObjectDefinition> mParentSceneObject;
        shared_ptr<SceneDefinition> mSceneDefinition;
        vector<shared_ptr<SceneObjectDefinition>> mChildDefinitions;
        Transform3D mTransform;

    public:
        SceneObjectDefinition(
            shared_ptr<SceneObjectDefinition> parent,
            shared_ptr<SceneDefinition> sceneDefinition,
            json data, bool randomUuid = false
        );

        ~SceneObjectDefinition() override;

        void setHasFocus(bool focus);
        bool hasFocus();

        void setFollowsCamera(bool fc);
        bool followsCamera();

        void addAssetDefinitionToLoadQueue(shared_ptr<IAssetDefinition> ad);
        void addAssetDefinitionUuidToLoadQueue(string uuid);

        void removeAssetDefinitionFromLoadQueue(shared_ptr<IAssetDefinition> ad);
        void removeAssetDefinitionUuidFromLoadQueue(string uuid);

        vector<string> getAssetDefinitionLoadQueue();

        Transform3D& getTransform();
        void setTransform(Transform3D& tform);

        void showStatus() override;

        vector<shared_ptr<SceneObjectDefinition>> getChildDefinitionsList();
        void addChildSceneObjectDefinition(shared_ptr<SceneObjectDefinition> child);
        void removeChildSceneObjectDefinition(shared_ptr<SceneObjectDefinition> child);
        shared_ptr<SceneObjectDefinition> createNewChildSceneObjectDefinition(json* def = nullptr);

        shared_ptr<SceneDefinition> getSceneDefinition();
        json getJson() override;

        shared_ptr<SceneObjectDefinition> getParentSceneObject() const;
        shared_ptr<SceneObjectDefinition> duplicate();

        bool getAlwaysDraw();
        void setAlwaysDraw(bool alwaysDraw);

        void setStatic(bool d);
        bool getStatic();

        void loadChildSceneObjectDefinitions(bool randomUuid = false);

    };

}
