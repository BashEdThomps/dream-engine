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

#include "../Common/Runtime.h"

using std::string;
using std::vector;

namespace Dream
{
    // Forward Declarations
    class Project;
    class PathComponent;
    class ScrollerComponent;
    class AnimationComponent;
    class AudioComponent;
    class InputComponent;
    class GraphicsComponent;
    class WindowComponent;
    class PhysicsComponent;
    class ScriptComponent;
    class NanoVGComponent;
    class Transform;
    class SceneRuntime;
    class SceneDefinition;
    class AudioCache;
    class MaterialCache;
    class TextureCache;
    class ScriptCache;
    class ModelCache;
    class ShaderCache;
    class ComponentThread;
    class AssetDefinition;
    class SceneObjectRuntime;
    class LifetimeComponent;

    class Time;

    // Class Declaration
    class ProjectRuntime : public Runtime
    {

    private: // Member Variables
        bool mDone;

        Time* mTime;
        Project* mProject;

        // Components
        AnimationComponent* mAnimationComponent;
        AudioComponent* mAudioComponent;
        InputComponent* mInputComponent;
        GraphicsComponent* mGraphicsComponent;
        NanoVGComponent* mNanoVGComponent;
        PhysicsComponent* mPhysicsComponent;
        PathComponent* mPathComponent;
        ScriptComponent* mScriptComponent;
        WindowComponent* mWindowComponent;
        ScrollerComponent* mScrollerComponent;
        LifetimeComponent* mLifetimeComponent;

        // Caches
        AudioCache* mAudioCache;
        TextureCache* mTextureCache;
        MaterialCache* mMaterialCache;
        ModelCache* mModelCache;
        ShaderCache* mShaderCache;
        ScriptCache* mScriptCache;
        bool mScriptingEnabled;

        vector<SceneRuntime*> mSceneRuntimeVector;
        vector<SceneRuntime*> mSceneRuntimesToRemove;

    public: // Public Functions
        ProjectRuntime(Project* parentProject, WindowComponent* wc = nullptr);
        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone();

        Time* getTime();

        AnimationComponent* getAnimationComponent();
        AudioComponent* getAudioComponent();
        PhysicsComponent* getPhysicsComponent();
        GraphicsComponent* getGraphicsComponent();
        NanoVGComponent* getNanoVGComponent();
        PathComponent* getPathComponent();
        WindowComponent* getWindowComponent();
        ScriptComponent* getScriptComponent();
        InputComponent* getInputComponent();
        ScrollerComponent* getScrollerComponent();
        LifetimeComponent* getLifetimeComponent();

        Project* getProject();

        bool initComponents();

        void collectGarbage(SceneRuntime* rt);
        void collectGarbage() override;

        void updateAll();
        bool updateLogic(SceneRuntime* rt);
        void updateGraphics(SceneRuntime* rt);

        int getWindowWidth();
        void setWindowWidth(int);

        int getWindowHeight();
        void setWindowHeight(int);

        bool constructSceneRuntime(SceneRuntime* rt);
        void destructSceneRuntime(SceneRuntime* rt, bool clearCaches = false);
        void clearAllCaches();

        bool useDefinition() override;

        AudioCache* getAudioCache();
        ShaderCache* getShaderCache();
        MaterialCache* getMaterialCache();
        ModelCache* getModelCache();
        TextureCache* getTextureCache();
        ScriptCache* getScriptCache();

        bool getScriptingEnabled() const;
        void setScriptingEnabled(bool);
        bool hasActiveScene();

        AssetDefinition* getAssetDefinitionByUuid(uint32_t uuid);

        SceneObjectRuntime* getSceneObjectRuntimeByUuid(SceneRuntime* rt, uint32_t uuid);
        SceneRuntime* getActiveSceneRuntime();
        SceneRuntime* getSceneRuntimeByUuid(uint32_t uuid);

        void addSceneRuntime(SceneRuntime*);
        void removeSceneRuntime(SceneRuntime*);
        void setSceneRuntimeAsActive(uint32_t uuid);
        vector<SceneRuntime*> getSceneRuntimeVector();
        bool hasSceneRuntime(uint32_t uuid);
        bool hasLoadedScenes();

    private: // Member Functions
        bool initPathComponent();
        bool initAnimationComponent();
        bool initAudioComponent();
        bool initInputComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initNanoVGComponent();
        bool initWindowComponent();
        bool initScriptComponent();
        bool initScrollerComponent();
        bool initLifetimeComponent();
        bool initCaches();

        void deleteCaches();
        void deleteComponents();
    };

} // End Dream
