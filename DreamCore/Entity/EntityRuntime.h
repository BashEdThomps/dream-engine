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

#include "EntityTasks.h"
#include "BoundingBox.h"
#include "Base/Runtime.h"
#include "Components/Graphics/Frustum.h"
#include "Math/Transform.h"

#include <vector>
#include <memory>

using std::vector;
using std::function;
using std::shared_ptr;
using std::weak_ptr;
using nlohmann::json;

namespace octronic::dream
{
    class Event;
    class EntityDefinition;
    class SceneRuntime;

    class AssetDefinition;
    class AssetRuntime;

    class AnimationRuntime;
    class AnimationDefinition;

    class AudioDefinition;
    class AudioRuntime;

    class FontDefinition;
    class FontRuntime;

    class ModelDefinition;
    class ModelRuntime;

    class PathDefinition;
    class PathRuntime;

    class PhysicsObjectDefinition;
    class PhysicsObjectRuntime;

    class ScriptDefinition;
    class ScriptRuntime;

    class TextureDefinition;
    class TextureRuntime;

    class EntityRuntime : public Runtime
    {
    public:
        EntityRuntime(
                const weak_ptr<ProjectRuntime>& pr,
                const weak_ptr<SceneRuntime>& sceneRuntime,
                const weak_ptr<EntityDefinition>& ed,
                bool randomUuid = false);
        ~EntityRuntime() override;

        void collectGarbage();

        weak_ptr<SceneRuntime> getSceneRuntime() const;
        weak_ptr<EntityDefinition> getEntityDefinition() const;

        bool createAssetRuntimes();
        bool createAnimationRuntime(const weak_ptr<AnimationDefinition>&);
        bool createPathRuntime(const weak_ptr<PathDefinition>&);
        bool createAudioRuntime(const weak_ptr<AudioDefinition>&);
        bool createModelRuntime(const weak_ptr<ModelDefinition>&);
        bool createScriptRuntime(const weak_ptr<ScriptDefinition>&);
        bool createPhysicsObjectRuntime(const weak_ptr<PhysicsObjectDefinition>&);
        bool createFontRuntime(const weak_ptr<FontDefinition>&);
        bool createTextureRuntime(const weak_ptr<TextureDefinition>&);

        weak_ptr<FontRuntime> getFontRuntime() const;
        weak_ptr<AnimationRuntime> getAnimationRuntime() const;
        weak_ptr<PathRuntime>  getPathRuntime() const;
        weak_ptr<AudioRuntime> getAudioRuntime() const;
        weak_ptr<ModelRuntime> getModelRuntime() const;
        weak_ptr<ScriptRuntime> getScriptRuntime() const;
        weak_ptr<PhysicsObjectRuntime> getPhysicsObjectRuntime() const;
        weak_ptr<AssetRuntime> getAssetRuntime(AssetType) const;
        weak_ptr<TextureRuntime> getTextureRuntime() const;

        bool hasAnimationRuntime() const;
        bool hasPathRuntime() const;
        bool hasAudioRuntime() const;
        bool hasModelRuntime() const;
        bool hasScriptRuntime() const;
        bool hasPhysicsObjectRuntime() const;
        bool hasFontRuntime() const;
        bool hasTextureRuntime() const;

        Transform getTransform() const;
        Transform getInitialTransform() const;
        void setTransform(const Transform& transform);

        bool hasEvents() const;
        void addEvent(const Event& event);
        vector<Event> getEventQueue();
        void clearProcessedEvents();

        weak_ptr<EntityRuntime> getChildRuntimeByUuid(UuidType uuid);
        weak_ptr<EntityRuntime> addChildFromTemplateUuid(UuidType uuid);
        int countAllChildren() const;
        size_t countChildren() const;
        void addChildRuntime(const shared_ptr<EntityRuntime>&);
        void removeChildRuntime(const weak_ptr<EntityRuntime>&);
        weak_ptr<EntityRuntime> createAndAddChildRuntime(const weak_ptr<EntityDefinition>&);
        vector<weak_ptr<EntityRuntime>> getChildRuntimes() const;
        bool isChildOf(const weak_ptr<EntityRuntime>&) const;

        void setScriptInitialised(bool i);
        bool getScriptInitialised() const;

        void setScriptError(bool i);
        bool getScriptError() const;

        bool isParentOf(const weak_ptr<EntityRuntime>& child) const;
        void setParentEntityRuntime(const weak_ptr<EntityRuntime>& parent);
        weak_ptr<EntityRuntime> getParentEntityRuntime() const;
        vector<weak_ptr<EntityRuntime>> generateFlatVector();

        bool loadFromDefinition() override;

        bool getDeleted() const;
        void setDeleted(bool deleted);

        void removeAnimationRuntime();
        void removeAudioRuntime();
        void removePathRuntime();
        void removeModelRuntime();
        void removeScriptRuntime();
        void removePhysicsObjectRuntime();
        void removeFontRuntime();
        void removeTextureRuntime();

        bool replaceAssetUuid(AssetType type, UuidType uuid);
        weak_ptr<AssetDefinition> getAssetDefinitionByUuid(UuidType uuid);
        void setAssetDefinitionsMap(const map<AssetType, UuidType> &loadQueue);
        map<AssetType, UuidType> getAssetDefinitionsMap() const;

        BoundingBox getBoundingBox() const;
        void setBoundingBox(const BoundingBox& boundingBox);

        float distanceFrom(const weak_ptr<EntityRuntime>& other) const;
        float distanceFrom(const vec3& other) const;
        bool visibleInFrustum();
        bool containedInFrustum();

        void initTransform();

        bool loadChildrenFromDefinition(const weak_ptr<EntityDefinition>& definition);

        weak_ptr<EntityScriptCreateStateTask> getScriptCreateStateTask() const;
        weak_ptr<EntityScriptOnInitTask> getScriptOnInitTask() const;
        weak_ptr<EntityScriptOnEventTask> getScriptOnEventTask() const;
        weak_ptr<EntityScriptOnUpdateTask> getScriptOnUpdateTask() const;

        string getAttribute(const string& key) const;
        void setAttribute(const string& key, const string& value);
        const map<string,string>& getAttributesMap() const;

        string getFontText() const;
        void setFontText(const string& fontText);

        vec4 getFontColor() const;
        void setFontColor(const vec4& fontColor);

        float getFontScale() const;
        void setFontScale(float fontScale);

        void pushTasks();
        bool allRuntimesLoaded() const;

    protected:
        weak_ptr<ProjectRuntime> mProjectRuntime;
        weak_ptr<SceneRuntime> mSceneRuntime;
        weak_ptr<EntityRuntime> mParentEntityRuntime;
        vector<shared_ptr<EntityRuntime>> mChildRuntimes;

        // Discrete Assets =====================================================
        shared_ptr<AnimationRuntime> mAnimationRuntime;
        shared_ptr<PathRuntime> mPathRuntime;
        shared_ptr<PhysicsObjectRuntime> mPhysicsObjectRuntime;

        // Shared Runtimes =====================================================
        // Audio
        weak_ptr<AudioRuntime> mAudioRuntime;

        // Font
        weak_ptr<FontRuntime> mFontRuntime;
        string mFontText;
        vec4 mFontColor;
		float mFontScale;

        // Model
        weak_ptr<ModelRuntime> mModelRuntime;

        // Script
        weak_ptr<ScriptRuntime> mScriptRuntime;
        bool mScriptError;
        bool mScriptInitialised;
        shared_ptr<EntityScriptCreateStateTask> mScriptCreateStateTask;
        shared_ptr<EntityScriptOnInitTask> mScriptOnInitTask;
        shared_ptr<EntityScriptOnUpdateTask> mScriptOnUpdateTask;
        shared_ptr<EntityScriptOnEventTask> mScriptOnEventTask;
        shared_ptr<EntityScriptRemoveStateTask> mScriptRemoveStateTask;

        // Sprite Texture
        weak_ptr<TextureRuntime> mTextureRuntime;

        // Transform
        Transform mInitialTransform;
        Transform mCurrentTransform;
        BoundingBox mBoundingBox;

        // Flags
        bool mDeleted;
        bool mRandomUuid;
        map<string,string> mAttributes;
        vector<Event> mEventQueue;
        map<AssetType,UuidType> mAssetDefinitions;
    };
}
