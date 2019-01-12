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
    class ScriptRuntime;
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
    class AssetRuntime;

    class SceneObjectRuntime : public Runtime
    {
    public:
        SceneObjectRuntime(SceneObjectDefinition* sd, SceneRuntime* sceneRuntime = nullptr, bool randomUuid = false);
        ~SceneObjectRuntime() override;

        void collectGarbage() override;

        SceneRuntime* getSceneRuntime();
        SceneObjectDefinition* getSceneObjectDefinition();

        bool createAssetInstances();
        bool createAnimationInstance(AnimationDefinition*);
        bool createPathInstance(PathDefinition*);
        bool createAudioInstance(AudioDefinition*);
        bool createModelInstance(ModelDefinition*);
        bool createScriptInstance(ScriptDefinition*);
        bool createPhysicsObjectInstance(PhysicsObjectDefinition*);
        bool createParticleEmitterInstance(ParticleEmitterDefinition*);
        bool createLightInstance(LightDefinition*);
        bool createScrollerInstance(ScrollerDefinition*);

        AnimationRuntime* getAnimationInstance();
        PathRuntime*  getPathInstance();
        AudioRuntime* getAudioInstance();
        ModelRuntime* getModelInstance();
        ScriptRuntime* getScriptInstance();
        PhysicsObjectRuntime* getPhysicsObjectInstance();
        LightRuntime* getLightInstance();
        ParticleEmitterRuntime* getParticleEmitterInstance();
        ScrollerRuntime* getScrollerInstance();
        AssetRuntime* getAssetInstance(AssetType);

        bool hasAnimationInstance();
        bool hasPathInstance();
        bool hasAudioInstance();
        bool hasModelInstance();
        bool hasScriptInstance();
        bool hasPhysicsObjectInstance();
        bool hasLightInstance();
        bool hasScrollerInstance();

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
        const vector<Event>& getEventQueue() const;
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

        bool getDeleted() const;
        void setDeleted(bool deleted);

        bool getHidden() const;
        void setHidden(bool hidden);

        void removeAnimationInstance();
        void removeAudioInstance();
        void removePathInstance();
        void removeModelInstance();
        void removeLightInstance();
        void removeScriptInstance();
        void removePhysicsObjectInstance();
        void removeParticleEmitterInstance();
        void removeScrollerInstance();

        bool replaceAssetUuid(AssetType type, uint32_t uuid);
        AssetDefinition* getAssetDefinitionByUuid(uint32_t uuid);
        void setAssetDefinitionsMap(map<AssetType,uint32_t> loadQueue);
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

    protected:
        void initTransform();
        bool loadChildrenFromDefinition(SceneObjectDefinition* definition);

    private:
        AnimationRuntime* mAnimationInstance;
        AudioRuntime* mAudioInstance;
        LightRuntime* mLightInstance;
        ParticleEmitterRuntime* mParticleEmitterInstance;
        PathRuntime* mPathInstance;
        PhysicsObjectRuntime* mPhysicsObjectInstance;
        ScriptRuntime* mScriptInstance;
        ModelRuntime* mModelInstance;
        ScrollerRuntime* mScrollerInstance;
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
    };
}
