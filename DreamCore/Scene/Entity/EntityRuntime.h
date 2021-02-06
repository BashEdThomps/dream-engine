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

#include "BoundingBox.h"
#include "Base/Runtime.h"
#include "Components/Script/ScriptTasks.h"
#include "Components/Graphics/Frustum.h"
#include "Components/Transform.h"

#include <vector>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;
using std::function;
using std::shared_ptr;
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

    class LightDefinition;
    class LightRuntime;

    class ModelDefinition;
    class ModelRuntime;

    class PathDefinition;
    class PathRuntime;

    class PhysicsObjectDefinition;
    class PhysicsObjectRuntime;

    class ScriptDefinition;
    class ScriptRuntime;

    class SpriteDefinition;
    class SpriteRuntime;



    class EntityRuntime : public Runtime
    {
    public:
        EntityRuntime(EntityDefinition* sd, SceneRuntime* sceneRuntime = nullptr, bool randomUuid = false);
        ~EntityRuntime() override;

        void collectGarbage() override;

        SceneRuntime* getSceneRuntime();
        EntityDefinition* getEntityDefinition();

        bool createAssetRuntimes();
        bool createAnimationRuntime(AnimationDefinition*);
        bool createPathRuntime(PathDefinition*);
        bool createAudioRuntime(AudioDefinition*);
        bool createModelRuntime(ModelDefinition*);
        bool createScriptRuntime(ScriptDefinition*);
        bool createPhysicsObjectRuntime(PhysicsObjectDefinition*);
        bool createLightRuntime(LightDefinition*);
        bool createFontRuntime(FontDefinition*);
        bool createSpriteRuntime(SpriteDefinition*);

        FontRuntime* getFontRuntime();
        AnimationRuntime* getAnimationRuntime();
        PathRuntime*  getPathRuntime();
        AudioRuntime* getAudioRuntime();
        ModelRuntime* getModelRuntime();
        ScriptRuntime* getScriptRuntime();
        PhysicsObjectRuntime* getPhysicsObjectRuntime();
        LightRuntime* getLightRuntime();
        SpriteRuntime* getSpriteRuntime();
        AssetRuntime* getAssetRuntime(AssetType);

        bool hasAnimationRuntime();
        bool hasPathRuntime();
        bool hasAudioRuntime();
        bool hasModelRuntime();
        bool hasScriptRuntime();
        bool hasPhysicsObjectRuntime();
        bool hasLightRuntime();
        bool hasFontRuntime();
        bool hasSpriteRuntime();

        Transform getTransform() const;
        Transform getInitialTransform();
        void setTransform(const Transform& transform);
        void translateWithChildren(const Vector3& translation);
        void preTranslateWithChildren(const Vector3& translation);
        void transformOffsetInitial(const mat4& matrix);
        void translateOffsetInitial(const Vector3& tx);

        bool getHasCameraFocus() const;
        void setHasCameraFocus(bool);

        bool isPlayerObject() const;
        bool hasEvents();
        void addEvent(const Event& event);
        vector<Event>* getEventQueue();
        bool tryLockEventQueue();
        void unlockEventQueue();
        void clearEventQueue();

        EntityRuntime* getChildRuntimeByUuid(UuidType uuid);
        EntityRuntime* addChildFromTemplateUuid(UuidType uuid);
        int countAllChildren();
        size_t countChildren();
        void addChildRuntime(EntityRuntime*);
        void removeChildRuntime(EntityRuntime*);
        EntityRuntime* createAndAddChildRuntime(EntityDefinition*);
        vector<EntityRuntime*> getChildRuntimes();
        bool isChildOf(EntityRuntime*);

        void setScriptInitialised(bool i);
        bool getScriptInitialised() const;

        void setScriptError(bool i);
        bool getScriptError() const;

        bool isParentOf(EntityRuntime* child);
        void setParentRuntime(EntityRuntime* parent);
        EntityRuntime* getParentRuntime();

        bool useDefinition() override;
        bool loadDeferred();

        bool getDeleted() const;
        void setDeleted(bool deleted);

        bool getHidden() const;
        void setHidden(bool hidden);

        void removeAnimationRuntime();
        void removeAudioRuntime();
        void removePathRuntime();
        void removeModelRuntime();
        void removeLightRuntime();
        void removeScriptRuntime();
        void removePhysicsObjectRuntime();
        void removeFontRuntime();
        void removeSpriteRuntime();

        bool replaceAssetUuid(AssetType type, UuidType uuid);
        AssetDefinition* getAssetDefinitionByUuid(UuidType uuid);
        void setAssetDefinitionsMap(const map<AssetType, UuidType> &loadQueue);
        map<AssetType, UuidType> getAssetDefinitionsMap();
        bool getAlwaysDraw() const;
        void setAlwaysDraw(bool alwaysDraw);

        BoundingBox& getBoundingBox();
        void setBoundingBox(const BoundingBox& boundingBox);

        float distanceFrom(EntityRuntime* other);
        float distanceFrom(const Vector3& other);
        bool visibleInFrustum();
        bool containedInFrustum();
        bool containedInFrustumAfterTransform(const mat4& tx);
        bool exceedsFrustumPlaneAtTranslation(Frustum::Plane plane, const Vector3& tx);

        bool applyToAll(const function<bool(EntityRuntime*)>& fn);
        EntityRuntime* applyToAll(const function<EntityRuntime*(EntityRuntime*)>& fn);
        void translateOffsetInitialWithChildren(const Vector3& translation);
        void initTransform();

        long getDeferredFor() const;
        void setDeferredFor(long deferred);

        long getObjectLifetime() const;
        void setObjectLifetime(long l);

        long getDieAfter() const;
        void setDieAfter(long);
        void updateLifetime();

        bool loadChildrenFromDefinition(EntityDefinition* definition);

        EntityScriptConstructionTask* getScriptConstructionTask();
        EntityScriptOnInitTask* getScriptOnInitTask();
        EntityScriptOnEventTask* getScriptOnEventTask();
        EntityScriptOnUpdateTask* getScriptOnUpdateTask();

        string getAttribute(const string& key) const;
        void setAttribute(const string& key, const string& value);
        const map<string,string>& getAttributesMap() const;

        string getFontText() const;
        void setFontText(const string& fontText);

        Vector3 getFontColor() const;
        void setFontColor(const Vector3& fontColor);

        float getFontScale() const;
        void setFontScale(float fontScale);

    protected:
        AnimationRuntime* mAnimationRuntime;
        AudioRuntime* mAudioRuntime;
        LightRuntime* mLightRuntime;
        PathRuntime* mPathRuntime;
        PhysicsObjectRuntime* mPhysicsObjectRuntime;
        ScriptRuntime* mScriptRuntime;
        bool mScriptInitialised;
        bool mScriptError;
        ModelRuntime* mModelRuntime;
        FontRuntime* mFontRuntime;
        SpriteRuntime* mSpriteRuntime;

        Transform mInitialTransform;
        Transform mTransform;
        mutex mEventQueueMutex;
        vector<Event> mEventQueue;
        map<AssetType,UuidType> mAssetDefinitions;
        vector<EntityRuntime*> mChildRuntimes;
        SceneRuntime* mSceneRuntime;
        EntityRuntime* mParentRuntime;
        BoundingBox mBoundingBox;
        bool mHasCameraFocus;
        bool mDeleted;
        bool mHidden;
        bool mAlwaysDraw;
        bool mRandomUuid;
        long mDeferredFor;
        long mObjectLifetime;
        long mDieAfter;
        EntityScriptConstructionTask mScriptConstructionTask;
        EntityScriptOnInitTask mScriptOnInitTask;
        EntityScriptOnUpdateTask mScriptOnUpdateTask;
        EntityScriptOnEventTask mScriptOnEventTask;
        shared_ptr<EntityScriptDestructionTask> mScriptDestructionTask;
        map<string,string> mAttributes;
        string mFontText;
        Vector3 mFontColor;
		float mFontScale;
    };
}
