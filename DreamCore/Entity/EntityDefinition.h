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

#include "Base/Definition.h"
#include "Math/Transform.h"

using glm::vec4;

namespace octronic::dream
{
    class AssetDefinition;
    class SceneDefinition;

    class EntityDefinition : public Definition
    {
    public:
        EntityDefinition(
            EntityDefinition* parent,
            SceneDefinition* sceneDefinition,
            const json& data,
            bool randomUuid = false
        );

        ~EntityDefinition() override;

        int getChildCount();

        Transform getTransform();
        void setTransform(Transform tform);

        vector<EntityDefinition*> getChildDefinitionsVector();
        vector<string> getChildNamesVector();
        void adoptChildDefinition(EntityDefinition* child);
        void addChildDefinition(EntityDefinition* child);
        void removeChildDefinition(EntityDefinition* child, bool andDelete = true);
        EntityDefinition* createNewChildDefinition(json* def = nullptr);

        SceneDefinition* getSceneDefinition();
        json toJson() override;

        EntityDefinition* getParentEntity();
        void setParentEntity(EntityDefinition* parentEntity);
        EntityDefinition* duplicate();

        void setIsTemplate(bool d);
        bool getIsTemplate();

        void loadChildEntityDefinitions(bool randomUuid = false);

        int getSelectedAssetIndex(AssetType type);
        void setSelectedAssetIndex(AssetType type, int index);

        UuidType getAssetDefinition(AssetType type);
        void setAssetDefinition(AssetType type, UuidType uuid);
        map<AssetType, UuidType> getAssetDefinitionsMap();

        void setFontColor(const vec4& color);
        vec4 getFontColor();

        void setFontText(const string& text);
        string getFontText();

        void setFontScale(float s);
        float getFontScale();

    private:
        void deleteChildEntityDefinitions();
        void setEmptyAssetsObject();
    private:
        EntityDefinition* mParentEntity;
        SceneDefinition* mSceneDefinition;
        vector<EntityDefinition*> mChildDefinitions;
    };
}
