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
#include <deque>

#include "../Common/Runtime.h"

using std::string;
using std::vector;
using std::deque;

namespace Dream
{
    // Forward Declarations
    class Project;
    class Transform;
    class AssetDefinition;
    class Time;

    class AudioComponent;
    class InputComponent;
    class GraphicsComponent;
    class WindowComponent;
    class PhysicsComponent;
    class ScriptComponent;
    class TaskManager;

    class SceneRuntime;
    class ActorRuntime;

    class SceneDefinition;

    class AudioCache;
    class MaterialCache;
    class TextureCache;
    class ScriptCache;
    class ModelCache;
    class ShaderCache;

    // Class Declaration
    class ProjectRuntime : public Runtime
    {

    private: // Member Variables
        bool mDone;

        Time* mTime;
        Project* mProject;

        // Components
        AudioComponent* mAudioComponent;
        InputComponent* mInputComponent;
        GraphicsComponent* mGraphicsComponent;
        PhysicsComponent* mPhysicsComponent;
        ScriptComponent* mScriptComponent;
        WindowComponent* mWindowComponent;
        TaskManager* mTaskManager;

        // Caches
        AudioCache* mAudioCache;
        TextureCache* mTextureCache;
        MaterialCache* mMaterialCache;
        ModelCache* mModelCache;
        ShaderCache* mShaderCache;
        ScriptCache* mScriptCache;

        vector<SceneRuntime*> mSceneRuntimeVector;
        vector<SceneRuntime*> mSceneRuntimesToRemove;
        deque<float> mFrameDurationHistory;

    public: // Public Functions
        static int MaxFrameCount;

        ProjectRuntime(Project* parentProject, WindowComponent* wc = nullptr);
        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone() const;

        Time* getTime() const;

        AudioComponent* getAudioComponent() const;
        PhysicsComponent* getPhysicsComponent() const;
        GraphicsComponent* getGraphicsComponent() const;
        WindowComponent* getWindowComponent() const;
        ScriptComponent* getScriptComponent() const;
        InputComponent* getInputComponent() const;
        TaskManager* getTaskManager() const;

        Project* getProject() const;

        bool initComponents();

        void collectGarbage(SceneRuntime* rt);
        void collectGarbage() override;

        void updateAll();
        bool updateLogic(SceneRuntime* rt);
        void updateGraphics(SceneRuntime* rt);

        int getWindowWidth() const;
        void setWindowWidth(int);

        int getWindowHeight() const;
        void setWindowHeight(int);

        bool constructSceneRuntime(SceneRuntime* rt);
        void destructSceneRuntime(SceneRuntime* rt, bool clearCaches = false);
        void clearAllCaches();

        bool useDefinition() override;

        AudioCache* getAudioCache() const;
        ShaderCache* getShaderCache() const;
        MaterialCache* getMaterialCache() const;
        ModelCache* getModelCache() const;
        TextureCache* getTextureCache() const;
        ScriptCache* getScriptCache() const;

        bool hasActiveScene() const;

        AssetDefinition* getAssetDefinitionByUuid(uint32_t uuid) const;

        ActorRuntime* getActorRuntimeByUuid(SceneRuntime* rt, uint32_t uuid) const;
        SceneRuntime* getActiveSceneRuntime() const;
        SceneRuntime* getSceneRuntimeByUuid(uint32_t uuid) const;

        void addSceneRuntime(SceneRuntime*);
        void removeSceneRuntime(SceneRuntime*);
        void setSceneRuntimeAsActive(uint32_t uuid);
        vector<SceneRuntime*> getSceneRuntimeVector() const;
        bool hasSceneRuntime(uint32_t uuid) const;
        bool hasLoadedScenes() const;

        deque<float> getFrameDurationHistory() const;
        float getAverageFramerate();

    private:
        bool initAudioComponent();
        bool initInputComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initWindowComponent();
        bool initScriptComponent();
        bool initTaskManager();
        bool initCaches();

        void deleteCaches();
        void deleteComponents();
    };
}
