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
using std::weak_ptr;
using std::shared_ptr;
using std::vector;

namespace Dream
{
    // Forward Declarations

    class Project;
    class PathComponent;
    class AudioComponent;
    class InputComponent;
    class GraphicsComponent;
    class NanoVGComponent;
    class IWindowComponent;
    class PhysicsComponent;
    class IScriptComponent;
    class Transform3D;
    class Camera;
    class SceneRuntime;
    class SceneDefinition;
    class MaterialCache;
    class ScriptCache;
    class AssimpCache;
    class FontCache;
    class ShaderCache;
    class ComponentThread;

    class Time;

    // Class Declaration
    class ProjectRuntime : public IRuntime
    {

    private: // Member Variables
        bool mDone;

        shared_ptr<Time> mTime;
        shared_ptr<Camera> mCamera;
        shared_ptr<SceneRuntime> mActiveSceneRuntime;
        weak_ptr<Project> mProject;

        // Components
        shared_ptr<AudioComponent> mAudioComponent;
        shared_ptr<InputComponent> mInputComponent;
        shared_ptr<GraphicsComponent> mGraphicsComponent;
        shared_ptr<PhysicsComponent> mPhysicsComponent;
        shared_ptr<PathComponent> mPathComponent;
        shared_ptr<IScriptComponent> mScriptComponent;
        shared_ptr<NanoVGComponent> mNanoVGComponent;
        shared_ptr<IWindowComponent> mWindowComponent;

        // Caches
        shared_ptr<MaterialCache> mTextureCache;
        shared_ptr<AssimpCache> mModelCache;
        shared_ptr<FontCache> mFontCache;
        shared_ptr<ShaderCache> mShaderCache;
        shared_ptr<ScriptCache> mScriptCache;
    public: // Public Functions
        ProjectRuntime(shared_ptr<Project> parentProject, shared_ptr<IWindowComponent> wc = nullptr);
        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone();

        weak_ptr<Camera> getCamera();
        weak_ptr<Time> getTime();

        weak_ptr<PathComponent> getPathComponent();
        weak_ptr<AudioComponent> getAudioComponent();
        weak_ptr<PhysicsComponent> getPhysicsComponent();
        weak_ptr<GraphicsComponent> getGraphicsComponent();
        weak_ptr<NanoVGComponent> getNanoVGComponent();
        weak_ptr<IWindowComponent> getWindowComponent();
        weak_ptr<IScriptComponent> getScriptComponent();
        weak_ptr<Project> getProject();
        weak_ptr<InputComponent> getInputComponent();

        bool initComponents();

        void collectGarbage() override;

        void updateAll();
        bool updateLogic();
        void updateGraphics();
        void updateFlush();
        bool allThreadsHaveUpdated();

        int getWindowWidth();
        void setWindowWidth(int);

        int getWindowHeight();
        void setWindowHeight(int);

        weak_ptr<SceneRuntime> constructActiveSceneRuntime
        (shared_ptr<SceneDefinition> sceneDefinition);

        bool hasActiveSceneRuntime();
        weak_ptr<SceneRuntime> getActiveSceneRuntime();
        void resetActiveSceneRuntime();

        void useDefinition(shared_ptr<IDefinition>) override;

        weak_ptr<FontCache> getFontCache();
        weak_ptr<ShaderCache> getShaderCache();
        weak_ptr<MaterialCache> getTextureCache();
        weak_ptr<AssimpCache> getModelCache();

    private: // Member Functions
        bool initPathComponent();
        bool initAudioComponent();
        bool initInputComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initWindowComponent();
        bool initScriptComponent();
        bool initCaches();
    };

} // End Dream
