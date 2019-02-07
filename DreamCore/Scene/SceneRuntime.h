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

#include <string>
#include <vector>

#include "SceneState.h"

#include "../Common/Runtime.h"
#include "../Components/Transform.h"
#include "../Components/Graphics/Camera.h"

using std::string;
using std::vector;
using std::shared_ptr;

namespace Dream
{
    class ProjectRuntime;
    class SceneObject;
    class SceneDefinition;
    class AssetRuntime;
    class AssetDefinition;
    class SceneObjectRuntime;
    class ShaderRuntime;
    class ScriptRuntime;

    class SceneRuntime : public Runtime
    {
    private:
        SceneState mState;
        vec3 mClearColour;
        ProjectRuntime* mProjectRuntime;
        vector<SceneObjectRuntime*> mSceneObjectRuntimeCleanUpQueue;
        SceneObjectRuntime* mRootSceneObjectRuntime;
        ShaderRuntime* mLightingPassShader;
        ShaderRuntime* mShadowPassShader;
        ScriptRuntime* mInputScript;
        ScriptRuntime* mNanoVGScript;
        Camera mCamera;
        double mSceneStartTime;
        double mSceneCurrentTime;
        float mMinDrawDistance;
        float mMaxDrawDistance;
        float mMeshCullDistance;

    public:
        SceneRuntime(SceneDefinition* sd, ProjectRuntime* parent);
        ~SceneRuntime() override;

        Camera* getCamera();
        SceneState getState() const;
        void setState(SceneState state);

        vec3 getGravity() const;
        void setGravity(const vec3& gravity);

        vec3 getClearColour() const;
        void setClearColour(const vec3& clearColour);

        vector<SceneObjectRuntime*> getSceneObjectRuntimeCleanUpQueue() const;
        void addSceneObjectRuntimeToCleanUpQueue(SceneObjectRuntime*);
        void clearSceneObjectRuntimeCleanUpQueue();
        void processSceneObjectRuntimeCleanUpQueue();
        void createSceneTasks();

        void createAllAssetRuntimes();

        bool useDefinition() override;
        void destroyRuntime();

        void setDeleteFlagOnAllSceneObjectRuntimes();

        bool hasRootSceneObjectRuntime() const;
        void setRootSceneObjectRuntime(SceneObjectRuntime* sceneObject);
        SceneObjectRuntime* getRootSceneObjectRuntime() const;

        SceneObjectRuntime* getSceneObjectRuntimeByName(const string& name) const;
        SceneObjectRuntime* getSceneObjectRuntimeByUuid(uint32_t uuid) const;

        int countSceneObjectRuntimes() const;
        int countChildrenOfSceneObjectRuntime(SceneObjectRuntime*) const;

        ProjectRuntime* getProjectRuntime() const;

        void showScenegraph() const;

        void collectGarbage() override;

        void setAssetDefinitionUuidLoadQueue(const vector<string>& loadQueue);

        bool getPhysicsDebug() const;
        void setPhysicsDebug(bool physicsDebug);

        ShaderRuntime* getLightingPassShader() const;
        void setLightingPassShader(ShaderRuntime* lightingShader);

        ShaderRuntime* getShadowPassShader() const;
        void setShadowPassShader(ShaderRuntime* shadowPassShader);

        void setMeshCullDistance(float);
        float getMeshCullDistance() const;

        float getMinDrawDistance() const;
        void setMinDrawDistance(float);

        float getMaxDrawDistance() const;
        void setMaxDrawDistance(float);


        vector<AssetRuntime*> getAssetRuntimes(AssetType) const;
        vector<SceneObjectRuntime*> getSceneObjectsWithRuntimeOf(AssetDefinition* def) const;

        /**
         * @return Gets the ScriptRuntime that handles Input
         */
        ScriptRuntime* getInputScript() const;
        /**
         * @return Gets the ScriptRuntime that controls the NanoVG Overlay
         */
        ScriptRuntime* getNanoVGScript() const;

        /**
         * @return Gets the nearest SceneObject to the Camera's position excluding
         * the SceneObject the Camera is focused on.
         */
        SceneObjectRuntime* getNearestToCamera() const;

        double getSceneCurrentTime() const;
        void setSceneCurrentTime(double sceneCurrentTime);

        double getSceneStartTime() const;
        void setSceneStartTime(double sceneStartTime);

    protected:
        void updateLifetime();
    };
}
