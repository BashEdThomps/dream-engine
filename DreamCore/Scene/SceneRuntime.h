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

#include "SceneState.h"

#include "Common/Runtime.h"
#include "Components/Transform.h"
#include "Components/Graphics/Camera.h"
#include "Components/Script/ScriptTasks.h"

#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::shared_ptr;

namespace octronic::dream
{
    class ProjectRuntime;
    class Entity;
    class SceneDefinition;
    class AssetRuntime;
    class AssetDefinition;
    class EntityRuntime;
    class ShaderRuntime;
    class ScriptRuntime;

    class SceneRuntime : public Runtime
    {


    public:
        SceneRuntime(SceneDefinition* sd, ProjectRuntime* parent);
        ~SceneRuntime() override;

        Camera* getCamera();
        SceneState getState() const;
        void setState(SceneState state);

        Vector3 getGravity() const;
        void setGravity(const Vector3& gravity);

        Vector3 getClearColour() const;
        void setClearColour(const Vector3& clearColour);

        bool useDefinition() override;
        void destroyRuntime();

        bool getPhysicsDebug() const;
        void setPhysicsDebug(bool physicsDebug);

        void setMeshCullDistance(float);
        float getMeshCullDistance() const;

        float getMinDrawDistance() const;
        void setMinDrawDistance(float);

        float getMaxDrawDistance() const;
        void setMaxDrawDistance(float);

        void createSceneTasks();

        bool hasRootEntityRuntime() const;
        void setRootEntityRuntime(EntityRuntime* e);
        EntityRuntime* getRootEntityRuntime() const;

        EntityRuntime* getEntityRuntimeByName(const string& name) const;
        EntityRuntime* getEntityRuntimeByUuid(UuidType uuid) const;

        int countEntityRuntimes() const;
        int countChildrenOfEntityRuntime(EntityRuntime*) const;

        void setAssetDefinitionUuidLoadQueue(const vector<string>& loadQueue);

        ProjectRuntime* getProjectRuntime() const;

        void showScenegraph() const;
        void collectGarbage() override;

        ShaderRuntime* getLightingPassShader() const;
        void setLightingPassShader(ShaderRuntime* lightingShader);

        ShaderRuntime* getShadowPassShader() const;
        void setShadowPassShader(ShaderRuntime* shadowPassShader);

        ShaderRuntime* getFontShader() const;
        void setFontShader(ShaderRuntime* fontShader);


        vector<AssetRuntime*> getAssetRuntimes(AssetType) const;
        vector<EntityRuntime*> getEntitysWithRuntimeOf(AssetDefinition* def) const;

        /**
         * @return Gets the ScriptRuntime that handles Input
         */
        ScriptRuntime* getInputScript() const;

        /**
         * @return Gets the nearest Entity to the Camera's position excluding
         * the Entity the Camera is focused on.
         */
        EntityRuntime* getNearestToCamera() const;

        unsigned long getSceneCurrentTime() const;
        void setSceneCurrentTime(unsigned long sceneCurrentTime);

        unsigned long getSceneStartTime() const;
        void setSceneStartTime(unsigned long sceneStartTime);

        void setPlayerEntity(EntityRuntime* po);
        EntityRuntime* getPlayerEntity() const;

    protected:
        void updateLifetime();
        vector<EntityRuntime*> getEntityRuntimeCleanUpQueue() const;
        void addEntityRuntimeToCleanUpQueue(EntityRuntime*);
        void clearEntityRuntimeCleanUpQueue();
        void processEntityRuntimeCleanUpQueue();
        void createAllAssetRuntimes();
        void setDeleteFlagOnAllEntityRuntimes();

    private:
        SceneState mState;
        Vector3 mClearColour;
        ProjectRuntime* mProjectRuntime;
        vector<EntityRuntime*> mEntityRuntimeCleanUpQueue;
        EntityRuntime* mRootEntityRuntime;
        ShaderRuntime* mLightingPassShader;
        ShaderRuntime* mShadowPassShader;
        ShaderRuntime* mFontShader;
        ScriptRuntime* mInputScript;
        InputScriptConstructionTask mInputScriptConstructionTask;
        shared_ptr<InputScriptDestructionTask> mInputScriptDestructionTask;
        Camera mCamera;
        unsigned long mSceneStartTime;
        unsigned long mSceneCurrentTime;
        float mMinDrawDistance;
        float mMaxDrawDistance;
        float mMeshCullDistance;
        EntityRuntime* mPlayerEntity;
    };
}
