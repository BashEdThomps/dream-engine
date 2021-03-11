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

#include "Base/Runtime.h"
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

    class SceneRuntime : public Runtime
    {
    public:
        SceneRuntime(ProjectRuntime* parent, SceneDefinition* sd);
        ~SceneRuntime() override;

        CameraRuntime* getCamera();

        SceneState getState() const;
        void setState(SceneState state);
        bool hasState(SceneState state) const;

        vec3 getGravity();
        void setGravity(const vec3& gravity);

        vec4 getClearColor();
        void setClearColor(const vec4& clear);

        bool loadFromDefinition() override;
        void destroyRuntime();



        void createSceneTasks();

        bool hasRootEntityRuntime() const;
        void setRootEntityRuntime(EntityRuntime* e);
        EntityRuntime* getRootEntityRuntime() ;

        EntityRuntime* getEntityRuntimeByName(const string& name) const;
        EntityRuntime* getEntityRuntimeByUuid(UuidType uuid) const;

        int countEntityRuntimes() const;
        int countChildrenOfEntityRuntime(EntityRuntime*) const;

        void setAssetDefinitionUuidLoadQueue(const vector<string>& loadQueue);

        ProjectRuntime* getProjectRuntime() const;

        void showScenegraph() const;
        void collectGarbage();

        ShaderRuntime* getShadowPassShader() const;
        void setShadowPassShader(ShaderRuntime* shadowPassShader);

        ShaderRuntime* getFontShader() const;
        void setFontShader(ShaderRuntime* shader);

		ShaderRuntime* getSpriteShader() const;
        void setSpriteShader(ShaderRuntime* shader);

        TextureRuntime* getEnvironmentTexture() const;
        void setEnvironmentTexture(TextureRuntime* );

        void setEnvironmentShader(ShaderRuntime* );
        ShaderRuntime* getEnvironmentShader() const;

        vector<AssetRuntime*> getAssetRuntimes(AssetType) const;
        vector<EntityRuntime*> getEntitysWithRuntimeOf(AssetDefinition* def) const;

        /**
         * @return Gets the nearest Entity to the Camera's position excluding
         * the Entity the Camera is focused on.
         */

        unsigned long getSceneCurrentTime() const;
        void setSceneCurrentTime(unsigned long sceneCurrentTime);

        unsigned long getSceneStartTime() const;
        void setSceneStartTime(unsigned long sceneStartTime);

        ScriptRuntime* getInputScript() const;

    protected:
        void updateLifetime();
        void createAllAssetRuntimes();
        void setDeleteFlagOnAllEntityRuntimes();

    private:
        SceneState mState;
        vec4 mClearColor;
        ProjectRuntime* mProjectRuntime;
        vector<EntityRuntime*> mEntityRuntimeCleanUpQueue;
        EntityRuntime* mRootEntityRuntime;
        ShaderRuntime* mShadowPassShader;
        ShaderRuntime* mFontShader;
        ShaderRuntime* mSpriteShader;
        TextureRuntime* mEnvironmentTexture;
        ShaderRuntime* mEnvironmentShader;
        ScriptRuntime* mInputScript;
        EntityRuntime* mCameraEntity;
        shared_ptr<CameraRuntime> mCamera;
        unsigned long mSceneStartTime;
        unsigned long mSceneCurrentTime;
    };
}
