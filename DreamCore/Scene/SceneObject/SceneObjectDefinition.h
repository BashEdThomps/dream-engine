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

#include "../../Common/Definition.h"
#include "../../Components/Transform.h"

using namespace std;

namespace Dream
{
    class AssetDefinition;
    class SceneDefinition;

    class SceneObjectDefinition : public Definition
    {
    private:
        SceneObjectDefinition* mParentSceneObject;
        SceneDefinition* mSceneDefinition;
        vector<SceneObjectDefinition*> mChildDefinitions;

    public:
        SceneObjectDefinition(
            SceneObjectDefinition* parent,
            SceneDefinition* sceneDefinition,
            json data,
            bool randomUuid = false
        );

        ~SceneObjectDefinition() override;

        int getChildCount() const;

        void setHasInputFocus(bool focus);
        bool getHasInputFocus();

        void setHasCameraFocus(bool fc);
        bool getHasCameraFocus();

        Transform getTransform();
        void setTransform(const Transform& tform);

        vector<SceneObjectDefinition*>& getChildDefinitionsList();
        void adoptChildDefinition(SceneObjectDefinition* child);
        void addChildDefinition(SceneObjectDefinition* child);
        void removeChildDefinition(SceneObjectDefinition* child, bool andDelete = true);
        SceneObjectDefinition* createNewChildDefinition(json* def = nullptr);

        SceneDefinition* getSceneDefinition();
        json getJson() override;

        SceneObjectDefinition* getParentSceneObject() const;
        void setParentSceneObject(SceneObjectDefinition* parentSceneObject);
        SceneObjectDefinition* duplicate();

        bool getAlwaysDraw();
        void setAlwaysDraw(bool alwaysDraw);

        void setStatic(bool d);
        bool getStatic();

        void loadChildSceneObjectDefinitions(bool randomUuid = false);
        void setHidden(bool d);
        bool getHidden();

        int getSelectedAssetIndex(AssetType type);
        void setSelectedAssetIndex(AssetType type, int index);

        string getAssetDefinition(AssetType type);
        void setAssetDefinition(AssetType type, string uuid);
        map<AssetType, string> getAssetDefinitionsMap();


    private:
        void deleteChildSceneObjectDefinitions();
        void setEmptyAssetsObject();
    };
}
