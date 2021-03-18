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
using std::shared_ptr;

namespace octronic::dream
{
    class AssetDefinition;
    class SceneDefinition;

    class EntityDefinition : public Definition
    {
    public:
        EntityDefinition(
            const weak_ptr<EntityDefinition>& parent,
            const weak_ptr<SceneDefinition>& sceneDefinition,
            const json& data,
            bool randomUuid = false
        );

        ~EntityDefinition() override;

        int getChildCount() const;

        Transform getTransform() const;
        void setTransform(const Transform& tform);

        vector<weak_ptr<EntityDefinition>> getChildDefinitionsVector() const;
        vector<string> getChildNamesVector() const;
        void adoptChildDefinition(const shared_ptr<EntityDefinition>& child);
        void addChildDefinition(const shared_ptr<EntityDefinition>& child);
        void removeChildDefinition(const weak_ptr<EntityDefinition>& child);
        shared_ptr<EntityDefinition> createNewChildDefinition(const json& def);

        weak_ptr<SceneDefinition> getSceneDefinition() const;
        json toJson() override;

        weak_ptr<EntityDefinition> getParentEntity() const;
        void setParentEntity(const weak_ptr<EntityDefinition>& parentEntity);
        shared_ptr<EntityDefinition> duplicate();

        void setIsTemplate(bool d);
        bool getIsTemplate() const;

        void loadChildEntityDefinitions(bool randomUuid = false);

        int getSelectedAssetIndex(AssetType type);
        void setSelectedAssetIndex(AssetType type, int index);

        UuidType getAssetDefinition(AssetType type) const;
        void setAssetDefinition(AssetType type, UuidType uuid);
        map<AssetType, UuidType> getAssetDefinitionsMap() const;

        void setFontColor(const vec4& color);
        vec4 getFontColor() const;

        void setFontText(const string& text);
        string getFontText() const;

        void setFontScale(float s);
        float getFontScale() const;

    private:
        void deleteChildEntityDefinitions();
    private:
        weak_ptr<EntityDefinition> mParentEntity;
        weak_ptr<SceneDefinition> mSceneDefinition;
        vector<shared_ptr<EntityDefinition>> mChildDefinitions;
    };
}
