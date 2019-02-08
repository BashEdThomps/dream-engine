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

#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BoundingBox.h"
#include "SceneObjectTasks.h"
#include "../../Components/Script/ScriptTasks.h"
#include "../../Components/Graphics/Frustum.h"
#include "../../Common/Runtime.h"
#include "../../Components/Transform.h"

using std::vector;
using std::function;
using nlohmann::json;
using glm::vec3;

namespace Dream
{
    class SceneObject;
    class Scene;
    class Event;
    class AnimationRuntime;
    class AudioRuntime;
    class PathRuntime;
    class ModelRuntime;
    class LightRuntime;
    class PhysicsObjectRuntime;
    class ParticleEmitterRuntime;
    class SceneRuntime;
    class SceneObjectDefinition;
    class AssetDefinition;
    class PathDefinition;
    class ScrollerRuntime;
    class ScrollerDefinition;
    class AnimationDefinition;
    class AudioDefinition;
    class LightDefinition;
    class ModelDefinition;
    class PhysicsObjectDefinition;
    class ScriptDefinition;
    class ParticleEmitterDefinition;
    class ScriptRuntime;
    class AssetRuntime;

    class SceneObjectRuntime : public Runtime
    {
    public:
        SceneObjectRuntime(SceneObjectDefinition* sd, SceneRuntime* sceneRuntime = nullptr, bool randomUuid = false);
        ~SceneObjectRuntime() override;

        void collectGarbage() override;

        SceneRuntime* getSceneRuntime();
        SceneObjectDefinition* getSceneObjectDefinition();

        bool createAssetRuntimes();
        bool createAnimationRuntime(AnimationDefinition*);
        bool createPathRuntime(PathDefinition*);
        bool createAudioRuntime(AudioDefinition*);
        bool createModelRuntime(ModelDefinition*);
        bool createScriptRuntime(ScriptDefinition*);
        bool createPhysicsObjectRuntime(PhysicsObjectDefinition*);
        bool createParticleEmitterRuntime(ParticleEmitterDefinition*);
        bool createLightRuntime(LightDefinition*);
        bool createScrollerRuntime(ScrollerDefinition*);

        AnimationRuntime* getAnimationRuntime();
        PathRuntime*  getPathRuntime();
        AudioRuntime* getAudioRuntime();
        ModelRuntime* getModelRuntime();
        ScriptRuntime* getScriptRuntime();
        PhysicsObjectRuntime* getPhysicsObjectRuntime();
        LightRuntime* getLightRuntime();
        ParticleEmitterRuntime* getParticleEmitterRuntime();
        ScrollerRuntime* getScrollerRuntime();
        AssetRuntime* getAssetRuntime(AssetType);

        bool hasAnimationRuntime();
        bool hasPathRuntime();
        bool hasAudioRuntime();
        bool hasModelRuntime();
        bool hasScriptRuntime();
        bool hasPhysicsObjectRuntime();
        bool hasLightRuntime();
        bool hasScrollerRuntime();

        Transform& getTransform();
        void setTransform(const Transform& transform);
        void translateWithChildren(const vec3& translation);
        void preTranslateWithChildren(const vec3& translation);
        void transformOffsetInitial(const mat4& matrix);
        void translateOffsetInitial(const vec3& tx);

        bool getHasCameraFocus() const;
        void setHasCameraFocus(bool);

        bool hasEvents() const;
        void addEvent(const Event& event);
        vector<Event>& getEventQueue();
        void clearEventQueue();

        SceneObjectRuntime* getChildRuntimeByUuid(uint32_t uuid);
        SceneObjectRuntime* addChildFromTemplateUuid(uint32_t uuid);
        int countAllChildren();
        size_t countChildren();
        void addChildRuntime(SceneObjectRuntime*);
        void removeChildRuntime(SceneObjectRuntime*);
        SceneObjectRuntime* createAndAddChildRuntime(SceneObjectDefinition*);
        vector<SceneObjectRuntime*> getChildRuntimes();
        bool isChildOf(SceneObjectRuntime*);

        bool isParentOf(SceneObjectRuntime* child);
        void setParentRuntime(SceneObjectRuntime* parent);
        SceneObjectRuntime* getParentRuntime();

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
        void removeParticleEmitterRuntime();
        void removeScrollerRuntime();

        bool replaceAssetUuid(AssetType type, uint32_t uuid);
        AssetDefinition* getAssetDefinitionByUuid(uint32_t uuid);
        void setAssetDefinitionsMap(const map<AssetType, uint32_t> &loadQueue);
        map<AssetType, uint32_t> getAssetDefinitionsMap();
        bool getAlwaysDraw() const;
        void setAlwaysDraw(bool alwaysDraw);

        BoundingBox& getBoundingBox();
        void setBoundingBox(const BoundingBox& boundingBox);

        float distanceFrom(SceneObjectRuntime* other);
        float distanceFrom(const vec3& other);
        bool visibleInFrustum();
        bool containedInFrustum();
        bool containedInFrustumAfterTransform(const mat4& tx);
        bool exceedsFrustumPlaneAtTranslation(Frustum::Plane plane, const vec3& tx);

        Transform& getInitialTransform();
        bool applyToAll(const function<bool(SceneObjectRuntime*)>& fn);
        SceneObjectRuntime* applyToAll(const function<SceneObjectRuntime*(SceneObjectRuntime*)>& fn);
        void translateOffsetInitialWithChildren(const vec3& translation);
        void initTransform();

        long getDeferredFor() const;
        void setDeferredFor(long deferred);

        long getObjectLifetime() const;
        void setObjectLifetime(long l);
        void increaseLifetime(long l);

        long getDieAfter() const;
        void setDieAfter(long);

        bool loadChildrenFromDefinition(SceneObjectDefinition* definition);

        LifetimeUpdateTask* getLifetimeUpdateTask();
        ScriptOnInitTask* getScriptOnInitTask();
        ScriptOnEventTask* getScriptOnEventTask();
        ScriptOnUpdateTask* getScriptOnUpdateTask();

    private:
        AnimationRuntime* mAnimationRuntime;
        AudioRuntime* mAudioRuntime;
        LightRuntime* mLightRuntime;
        ParticleEmitterRuntime* mParticleEmitterRuntime;
        PathRuntime* mPathRuntime;
        PhysicsObjectRuntime* mPhysicsObjectRuntime;
        ScriptRuntime* mScriptRuntime;
        ModelRuntime* mModelRuntime;
        ScrollerRuntime* mScrollerRuntime;

        Transform mInitialTransform;
        Transform mTransform;
        vector<Event> mEventQueue;
        map<AssetType,uint32_t> mAssetDefinitions;
        vector<SceneObjectRuntime*> mChildRuntimes;
        SceneRuntime* mSceneRuntime;
        SceneObjectRuntime* mParentRuntime;
        BoundingBox mBoundingBox;
        bool mHasCameraFocus;
        bool mDeleted;
        bool mHidden;
        bool mAlwaysDraw;
        bool mRandomUuid;
        long mDeferredFor;
        long mObjectLifetime;
        long mDieAfter;
        LifetimeUpdateTask mLifetimeUpdateTask;
        ScriptOnInitTask mScriptOnInitTask;
        ScriptOnUpdateTask mScriptOnUpdateTask;
        ScriptOnEventTask mScriptOnEventTask;
    };
}
