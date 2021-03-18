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

#include "Base/DeferredLoadRuntime.h"
#include "Math/Transform.h"
#include "Components/Graphics/CameraRuntime.h"

#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::shared_ptr;

namespace octronic::dream
{
    class AssetDefinition;
    class SceneDefinition;

    class AssetRuntime;
    class EntityRuntime;
    class ProjectRuntime;
    class ScriptRuntime;
    class ShaderRuntime;
    class TextureRuntime;

    class SceneRuntime : public DeferredLoadRuntime
    {
    public:
        SceneRuntime(const weak_ptr<ProjectRuntime>& parent,
                     const weak_ptr<SceneDefinition>& sd);
        ~SceneRuntime() override;

        weak_ptr<CameraRuntime> getCamera() const;

        SceneState getState() const;
        void setState(SceneState state);
        bool hasState(SceneState state) const;

        vec3 getGravity() const;
        void setGravity(const vec3& gravity);

        vec4 getClearColor() const;
        void setClearColor(const vec4& clear);

        bool loadFromDefinition() override;
        void destroyRuntime();

        void createSceneTasks();

        bool hasRootEntityRuntime() const;
        void setRootEntityRuntime(const shared_ptr<EntityRuntime>& e);
        weak_ptr<EntityRuntime> getRootEntityRuntime() const;
        weak_ptr<EntityRuntime> getEntityRuntimeByName(const string& name) const;
        weak_ptr<EntityRuntime> getEntityRuntimeByUuid(UuidType uuid) const;

        int countEntityRuntimes() const;
        int countChildrenOfEntityRuntime(const weak_ptr<EntityRuntime>&) const;

        void setAssetDefinitionUuidLoadQueue(const vector<string>& loadQueue);

        weak_ptr<ProjectRuntime> getProjectRuntime() const;

        void showScenegraph() const;
        void collectGarbage();

        weak_ptr<ShaderRuntime> getShadowPassShader() const;
        void setShadowPassShader(const weak_ptr<ShaderRuntime>& shadowPassShader);

        weak_ptr<ShaderRuntime> getFontShader() const;
        void setFontShader(const weak_ptr<ShaderRuntime>& shader);

		weak_ptr<ShaderRuntime> getSpriteShader() const;
        void setSpriteShader(const weak_ptr<ShaderRuntime>& shader);

        weak_ptr<TextureRuntime> getEnvironmentTexture() const;
        void setEnvironmentTexture(const weak_ptr<TextureRuntime>& );

        weak_ptr<ShaderRuntime> getEnvironmentShader() const;
        void setEnvironmentShader(const weak_ptr<ShaderRuntime>&);

        vector<weak_ptr<AssetRuntime>> getAssetRuntimes(AssetType) const;
        vector<weak_ptr<EntityRuntime>> getEntitiesWithRuntimeOf(const weak_ptr<AssetDefinition>& def) const;
        vector<weak_ptr<EntityRuntime>> getFlatVector() const;

        /**
         * @return Gets the nearest Entity to the Camera's position excluding
         * the Entity the Camera is focused on.
         */

        unsigned long getSceneCurrentTime() const;
        void setSceneCurrentTime(unsigned long sceneCurrentTime);

        unsigned long getSceneStartTime() const;
        void setSceneStartTime(unsigned long sceneStartTime);

        weak_ptr<ScriptRuntime> getInputScript() const;

        void updateFlatVector();

    protected:
        void updateLifetime();
        void createAllAssetRuntimes();
        void setDeleteFlagOnAllEntityRuntimes();

    private:
        SceneState mState;
        vec4 mClearColor;
        weak_ptr<ProjectRuntime> mProjectRuntime;
        vector<weak_ptr<EntityRuntime>> mEntityRuntimeCleanUpQueue;
        shared_ptr<EntityRuntime> mRootEntityRuntime;
        vector<weak_ptr<EntityRuntime>> mFlatVector;
        weak_ptr<ShaderRuntime> mShadowPassShader;
        weak_ptr<ShaderRuntime> mFontShader;
        weak_ptr<ShaderRuntime> mSpriteShader;
        weak_ptr<TextureRuntime> mEnvironmentTexture;
        weak_ptr<ShaderRuntime> mEnvironmentShader;
        weak_ptr<ScriptRuntime> mInputScript;
        weak_ptr<EntityRuntime> mCameraEntity;
        shared_ptr<CameraRuntime> mCamera;
        unsigned long mSceneStartTime;
        unsigned long mSceneCurrentTime;
    };
}
