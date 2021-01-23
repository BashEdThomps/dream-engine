/*
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

#include "Common/Definition.h"
#include "Components/Transform.h"

using namespace std;

namespace octronic::dream
{
    class AssetDefinition;
    class SceneDefinition;

    class EntityDefinition : public Definition
    {
    private:
        EntityDefinition* mParentEntity;
        SceneDefinition* mSceneDefinition;
        vector<EntityDefinition*> mChildDefinitions;

    public:
        EntityDefinition(
            EntityDefinition* parent,
            SceneDefinition* sceneDefinition,
            const json& data,
            bool randomUuid = false
        );

        ~EntityDefinition() override;

        int getChildCount() const;

        void setHasCameraFocus(bool fc);
        bool getHasCameraFocus();

        Transform getTransform();
        void setTransform(const Transform& tform);

        vector<EntityDefinition*>& getChildDefinitionsList();
        void adoptChildDefinition(EntityDefinition* child);
        void addChildDefinition(EntityDefinition* child);
        void removeChildDefinition(EntityDefinition* child, bool andDelete = true);
        EntityDefinition* createNewChildDefinition(json* def = nullptr);

        SceneDefinition* getSceneDefinition();
        json getJson() override;

        EntityDefinition* getParentEntity() const;
        void setParentEntity(EntityDefinition* parentEntity);
        EntityDefinition* duplicate();

        bool getAlwaysDraw();
        void setAlwaysDraw(bool alwaysDraw);

        void setIsTemplate(bool d);
        bool getIsTemplate();

        long getDeferred();
        void setDeferred(long);

        long getDieAfter();
        void setDieAfter(long d);

        void loadChildEntityDefinitions(bool randomUuid = false);
        void setHidden(bool d);
        bool getHidden();

        int getSelectedAssetIndex(AssetType type);
        void setSelectedAssetIndex(AssetType type, int index);

        uint32_t getAssetDefinition(AssetType type);
        void setAssetDefinition(AssetType type, uint32_t uuid);
        map<AssetType, uint32_t> getAssetDefinitionsMap();


    private:
        void deleteChildEntityDefinitions();
        void setEmptyAssetsObject();
    };
}
