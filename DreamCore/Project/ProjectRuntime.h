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

// Base
#include "Base/Runtime.h"
// Time
#include "Components/Time.h"
// Audio
#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Audio/AudioRuntime.h"
// Input
#include "Components/Input/InputComponent.h"
// Graphics
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Font/FontRuntime.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Material/MaterialRuntime.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Graphics/Model/ModelRuntime.h"
// Physics
#include "Components/Physics/PhysicsComponent.h"
// Script
#include "Components/Script/ScriptComponent.h"
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptRuntime.h"
// Task
#include "Task/Task.h"
#include "Task/TaskQueue.h"
// STD
#include <string>
#include <vector>
#include <deque>
#include <memory>

// Using
using std::string;
using std::vector;
using std::deque;
using std::unique_ptr;

namespace octronic::dream
{
    // Typedef
    template <typename DefinitionType, typename RuntimeType> class Cache;
 	typedef Cache<AudioDefinition,AudioRuntime>       AudioCache;
	typedef Cache<FontDefinition,FontRuntime>         FontCache;
	typedef Cache<MaterialDefinition,MaterialRuntime> MaterialCache;
	typedef Cache<ModelDefinition,ModelRuntime>       ModelCache;
	typedef Cache<ScriptDefinition,ScriptRuntime>     ScriptCache;
	typedef Cache<ShaderDefinition,ShaderRuntime>     ShaderCache;
	typedef Cache<TextureDefinition,TextureRuntime>   TextureCache;

    // Forward Declarations
    class ProjectDirectory;
    class AssetDefinition;
    class WindowComponent;
    class StorageManager;
    class SceneRuntime;
    class EntityRuntime;
    class SceneDefinition;

    // Class Declaration
    class ProjectRuntime : public Runtime
    {
    public: // Public Variables
        static int MaxFrameCount;

    public: // Public Functions

        ProjectRuntime(const weak_ptr<ProjectDefinition>& parentProject,
                       const weak_ptr<ProjectDirectory>& directory,
                       const shared_ptr<WindowComponent>& wc,
                       const shared_ptr<AudioComponent>& ac,
                       const shared_ptr<StorageManager>& fm);

        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone() const;

        weak_ptr<ProjectDirectory> getProjectDirectory() const;

        // Components ==========================================================

        bool initComponents();
        weak_ptr<Time>              getTime() const;
        weak_ptr<AudioComponent>    getAudioComponent() const;
        weak_ptr<PhysicsComponent>  getPhysicsComponent() const;
        weak_ptr<GraphicsComponent> getGraphicsComponent() const;
        weak_ptr<WindowComponent>   getWindowComponent() const;
        weak_ptr<ScriptComponent>   getScriptComponent() const;
        weak_ptr<InputComponent>    getInputComponent() const;
        weak_ptr<StorageManager>    getStorageManager() const;
        weak_ptr<TaskQueue<Task>>   getTaskQueue() const;
        weak_ptr<TaskQueue<DestructionTask>> getDestructionTaskQueue() const;

        // Running =============================================================

        bool loadFromDefinition() override;
        void collectGarbage();
        void step();


        // Caches ==============================================================

        weak_ptr<AudioCache>    getAudioCache() const;
        weak_ptr<ShaderCache>   getShaderCache() const;
        weak_ptr<MaterialCache> getMaterialCache() const;
        weak_ptr<ModelCache>    getModelCache() const;
        weak_ptr<TextureCache>  getTextureCache() const;
        weak_ptr<ScriptCache>   getScriptCache() const;
        weak_ptr<FontCache>     getFontCache() const;
        bool initCaches();
        void clearAllCaches();

        // Scenes ==============================================================

        weak_ptr<AssetDefinition> getAssetDefinitionByUuid(UuidType uuid) const;
        weak_ptr<SceneRuntime> getActiveSceneRuntime() const;
        weak_ptr<SceneRuntime> getSceneRuntimeByUuid(UuidType uuid) const;
        bool constructSceneRuntime(const weak_ptr<SceneRuntime>& rt);
        void destructSceneRuntime(const weak_ptr<SceneRuntime>& rt);

        bool hasActiveScene() const;
        void addSceneRuntime(const shared_ptr<SceneRuntime>&);
        void removeSceneRuntime(const weak_ptr<SceneRuntime>&);
        void setActiveSceneRuntime(UuidType uuid);
        vector<weak_ptr<SceneRuntime>> getSceneRuntimeVector() const;
        bool hasSceneRuntime(UuidType uuid) const;
        bool hasLoadedScenes() const;

        // Frames ==============================================================

        deque<float> getFrameDurationHistory() const;
        float getAverageFramerate();

    private: // Member Functions
        bool initAudioComponent();
        bool initInputComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initWindowComponent();
        bool initScriptComponent();

		void pushComponentTasks();

    private: // Member Variables
        bool mDone;
        weak_ptr<ProjectDirectory> mProjectDirectory;

        // Passed in Components, we take ownership
        shared_ptr<AudioComponent>  mAudioComponent;
        shared_ptr<WindowComponent> mWindowComponent;
        shared_ptr<StorageManager>  mStorageManager;

        // Project created Components
        shared_ptr<Time> mTime;
        shared_ptr<InputComponent>    mInputComponent;
        shared_ptr<GraphicsComponent> mGraphicsComponent;
        shared_ptr<PhysicsComponent>  mPhysicsComponent;
        shared_ptr<ScriptComponent>   mScriptComponent;

        // Caches, Project owns those too
        shared_ptr<AudioCache>     mAudioCache;
        shared_ptr<FontCache>      mFontCache;
        shared_ptr<MaterialCache>  mMaterialCache;
        shared_ptr<ModelCache>     mModelCache;
        shared_ptr<ScriptCache>    mScriptCache;
        shared_ptr<ShaderCache>    mShaderCache;
        shared_ptr<TextureCache>   mTextureCache;

        // SceneRuntime
        weak_ptr<SceneRuntime> mActiveSceneRuntime;
        vector<shared_ptr<SceneRuntime>> mSceneRuntimeVector;
        vector<weak_ptr<SceneRuntime>> mSceneRuntimesToRemove;

        // Tasking
        shared_ptr<TaskQueue<Task>> mTaskQueue;
        shared_ptr<TaskQueue<DestructionTask>> mDestructionTaskQueue;

        // Frames
        deque<float> mFrameDurationHistory;
    };
}
