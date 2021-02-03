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
        SceneState getState();
        void setState(SceneState state);

        Vector3 getGravity();
        void setGravity(const Vector3& gravity);

        Vector3 getClearColour();
        void setClearColour(const Vector3& clearColour);

        bool useDefinition() override;
        void destroyRuntime();

        bool getPhysicsDebug();
        void setPhysicsDebug(bool physicsDebug);

        void setMeshCullDistance(float);
        float getMeshCullDistance();

        float getMinDrawDistance();
        void setMinDrawDistance(float);

        float getMaxDrawDistance();
        void setMaxDrawDistance(float);

        void createSceneTasks();

        bool hasRootEntityRuntime();
        void setRootEntityRuntime(EntityRuntime* e);
        EntityRuntime* getRootEntityRuntime() ;

        EntityRuntime* getEntityRuntimeByName(const string& name) ;
        EntityRuntime* getEntityRuntimeByUuid(UuidType uuid) ;

        int countEntityRuntimes() ;
        int countChildrenOfEntityRuntime(EntityRuntime*) ;

        void setAssetDefinitionUuidLoadQueue(const vector<string>& loadQueue);

        ProjectRuntime* getProjectRuntime() ;

        void showScenegraph() ;
        void collectGarbage() override;

        ShaderRuntime* getLightingPassShader() ;
        void setLightingPassShader(ShaderRuntime* lightingShader);

        ShaderRuntime* getShadowPassShader() ;
        void setShadowPassShader(ShaderRuntime* shadowPassShader);

        ShaderRuntime* getFontShader() ;
        void setFontShader(ShaderRuntime* fontShader);


        vector<AssetRuntime*> getAssetRuntimes(AssetType) ;
        vector<EntityRuntime*> getEntitysWithRuntimeOf(AssetDefinition* def) ;

        /**
         * @return Gets the ScriptRuntime that handles Input
         */
        ScriptRuntime* getInputScript() ;

        /**
         * @return Gets the nearest Entity to the Camera's position excluding
         * the Entity the Camera is focused on.
         */
        EntityRuntime* getNearestToCamera() ;

        unsigned long getSceneCurrentTime() ;
        void setSceneCurrentTime(unsigned long sceneCurrentTime);

        unsigned long getSceneStartTime() ;
        void setSceneStartTime(unsigned long sceneStartTime);

        void setPlayerEntity(EntityRuntime* po);
        EntityRuntime* getPlayerEntity() ;

    protected:
        void updateLifetime();
        vector<EntityRuntime*> getEntityRuntimeCleanUpQueue() ;
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
