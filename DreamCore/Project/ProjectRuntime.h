/*
 * ProjectRuntime.h
 *
 * Created: 05 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

#include <memory>
#include <string>
#include <vector>

#include "../Common/IRuntime.h"

using std::string;
using std::vector;


namespace Dream
{
    // Forward Declarations

    class Project;
    class PathComponent;
    class AudioComponent;
    class InputComponent;
    class GraphicsComponent;
    class IWindowComponent;
    class PhysicsComponent;
    class IScriptComponent;
    class NanoVGComponent;
    class Transform3D;
    class Camera;
    class SceneRuntime;
    class SceneDefinition;
    class MaterialCache;
    class TextureCache;
    class ScriptCache;
    class ModelCache;
    class ShaderCache;
    class ComponentThread;
    class IAssetDefinition;
    class SceneObjectRuntime;

    class Time;

    // Class Declaration
    class ProjectRuntime : public IRuntime
    {

    private: // Member Variables
        bool mDone;

        Time* mTime;
        Camera* mCamera;
        Project* mProject;

        // Components
        AudioComponent* mAudioComponent;
        InputComponent* mInputComponent;
        GraphicsComponent* mGraphicsComponent;
        NanoVGComponent* mNanoVGComponent;
        PhysicsComponent* mPhysicsComponent;
        PathComponent* mPathComponent;
        IScriptComponent* mScriptComponent;
        IWindowComponent* mWindowComponent;

        // Caches
        TextureCache* mTextureCache;
        MaterialCache* mMaterialCache;
        ModelCache* mModelCache;
        ShaderCache* mShaderCache;
        ScriptCache* mScriptCache;
        bool mScriptingEnabled;

    public: // Public Functions
        ProjectRuntime(
            Project* parentProject,
            IWindowComponent* wc = nullptr);
        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone();

        Camera* getCamera();
        Time* getTime();

        PathComponent* getPathComponent();
        AudioComponent* getAudioComponent();
        PhysicsComponent* getPhysicsComponent();
        GraphicsComponent* getGraphicsComponent();
        NanoVGComponent* getNanoVGComponent();
        IWindowComponent* getWindowComponent();
        IScriptComponent* getScriptComponent();
        Project* getProject();
        InputComponent* getInputComponent();

        bool initComponents();

        void collectGarbage(SceneRuntime* rt);
        void collectGarbage() override;

        void updateAll(SceneRuntime* rt);
        bool updateLogic(SceneRuntime* rt);
        void updateGraphics(SceneRuntime* rt);

        int getWindowWidth();
        void setWindowWidth(int);

        int getWindowHeight();
        void setWindowHeight(int);

        SceneRuntime* constructSceneRuntime(SceneDefinition* sceneDefinition);
        void destructSceneRuntime(SceneRuntime* rt, bool clearCaches = true);
        void clearAllCaches();

        bool useDefinition() override;

        ShaderCache* getShaderCache();
        MaterialCache* getMaterialCache();
        ModelCache* getModelCache();
        TextureCache* getTextureCache();

        bool getScriptingEnabled() const;
        void setScriptingEnabled(bool);

        IAssetDefinition* getAssetDefinitionByUuid(string uuid);
        string getAssetAbsolutePath(string uuid);

        SceneObjectRuntime* getSceneObjectRuntimeByUuid(SceneRuntime* rt, string uuid);

        string getProjectPath();
    private: // Member Functions
        bool initPathComponent();
        bool initAudioComponent();
        bool initInputComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initNanoVGComponent();
        bool initWindowComponent();
        bool initScriptComponent();
        bool initCaches();

        void deleteCaches();
        void deleteComponents();
    };

} // End Dream
