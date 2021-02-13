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
    class Project;
    class Transform;
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

        ProjectRuntime(Project* parentProject, WindowComponent* wc, AudioComponent* ac, StorageManager* fm);
        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone() const;

        Time* getTime();

        AudioComponent*    getAudioComponent();
        PhysicsComponent*  getPhysicsComponent();
        GraphicsComponent* getGraphicsComponent();
        WindowComponent*   getWindowComponent();
        ScriptComponent*   getScriptComponent();
        InputComponent*    getInputComponent();
        StorageManager*    getStorageManager();
        TaskQueue<shared_ptr<Task>>* getTaskQueue();
        TaskQueue<shared_ptr<DestructionTask>>* getDestructionTaskQueue();

        Project* getProject() const;

        bool initComponents();

        void collectGarbage();

        void step();

        int getWindowWidth() const;
        void setWindowWidth(int);

        int getWindowHeight() const;
        void setWindowHeight(int);

        bool constructSceneRuntime(SceneRuntime* rt);
        void destructSceneRuntime(SceneRuntime* rt, bool clearCaches = false);
        void clearAllCaches();

        bool loadFromDefinition() override;

        shared_ptr<Cache<AudioDefinition, AudioRuntime>>       getAudioCache();
        shared_ptr<Cache<ShaderDefinition, ShaderRuntime>>     getShaderCache();
        shared_ptr<Cache<MaterialDefinition, MaterialRuntime>> getMaterialCache();
        shared_ptr<Cache<ModelDefinition, ModelRuntime>>       getModelCache();
        shared_ptr<Cache<TextureDefinition, TextureRuntime>>   getTextureCache();
        shared_ptr<Cache<ScriptDefinition, ScriptRuntime>>     getScriptCache();
        shared_ptr<Cache<FontDefinition, FontRuntime>>         getFontCache();

        bool hasActiveScene() const;

        AssetDefinition* getAssetDefinitionByUuid(UuidType uuid) const;

        EntityRuntime* getEntityRuntimeByUuid(SceneRuntime* rt, UuidType uuid) const;
        SceneRuntime* getActiveSceneRuntime() const;
        SceneRuntime* getSceneRuntimeByUuid(UuidType uuid) const;

        void addSceneRuntime(SceneRuntime*);
        void removeSceneRuntime(SceneRuntime*);
        void setActiveSceneRuntime(UuidType uuid);
        vector<SceneRuntime*> getSceneRuntimeVector() const;
        bool hasSceneRuntime(UuidType uuid) const;
        bool hasLoadedScenes() const;

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
        bool     mDone;
        Time     mTime;
        Project* mProjectHandle;

        // Component Handles
        AudioComponent*  mAudioComponentHandle;
        WindowComponent* mWindowComponentHandle;
        StorageManager*  mStorageManagerHandle;

        // Component Handles
        InputComponent    mInputComponent;
        GraphicsComponent mGraphicsComponent;
        PhysicsComponent  mPhysicsComponent;
        ScriptComponent   mScriptComponent;

        // Caches
        shared_ptr<AudioCache>     mAudioCache;
        shared_ptr<FontCache>      mFontCache;
        shared_ptr<MaterialCache>  mMaterialCache;
        shared_ptr<ModelCache>     mModelCache;
        shared_ptr<ScriptCache>    mScriptCache;
        shared_ptr<ShaderCache>    mShaderCache;
        shared_ptr<TextureCache>   mTextureCache;

        // SceneRuntime
        SceneRuntime*         mActiveSceneRuntime;
        vector<SceneRuntime*> mSceneRuntimeVector;
        vector<SceneRuntime*> mSceneRuntimesToRemove;
        deque<float>          mFrameDurationHistory;

        // Tasking
        TaskQueue<shared_ptr<Task>>            mTaskQueue;
        TaskQueue<shared_ptr<DestructionTask>> mDestructionTaskQueue;
    };
}
