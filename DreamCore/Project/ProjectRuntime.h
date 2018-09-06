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
    class LuaComponent;
    class Transform3D;
    class Camera;
    class SceneRuntime;
    class SceneDefinition;
    class MaterialCache;
    class LuaScriptCache;
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
        bool mParallel;

        shared_ptr<Time> mTime;
        shared_ptr<Camera> mCamera;

        shared_ptr<AudioComponent> mAudioComponent;
        shared_ptr<ComponentThread> mAudioComponentThread;

        shared_ptr<InputComponent> mInputComponent;
        shared_ptr<ComponentThread> mInputComponentThread;

        shared_ptr<GraphicsComponent> mGraphicsComponent;
        shared_ptr<ComponentThread> mGraphicsComponentThread;

        shared_ptr<PhysicsComponent> mPhysicsComponent;
        shared_ptr<ComponentThread> mPhysicsComponentThread;

        shared_ptr<PathComponent> mPathComponent;
        shared_ptr<ComponentThread> mPathComponentThread;

        shared_ptr<LuaComponent> mLuaComponent;
        shared_ptr<ComponentThread> mLuaComponentThread;

        shared_ptr<NanoVGComponent> mNanoVGComponent;

        // Caches

        shared_ptr<MaterialCache> mTextureCache;
        shared_ptr<AssimpCache> mModelCache;
        shared_ptr<FontCache> mFontCache;
        shared_ptr<ShaderCache> mShaderCache;
        shared_ptr<LuaScriptCache> mScriptCache;

        shared_ptr<IWindowComponent> mWindowComponent;
        shared_ptr<SceneRuntime> mActiveSceneRuntime;
        shared_ptr<Project> mProject;

    public: // Public Functions
        ProjectRuntime(shared_ptr<Project> parentProject, shared_ptr<IWindowComponent> wc = nullptr);
        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone();

        shared_ptr<Camera> getCamera();
        shared_ptr<Time> getTime();

        shared_ptr<PathComponent> getPathComponent();
        shared_ptr<AudioComponent> getAudioComponent();
        shared_ptr<PhysicsComponent> getPhysicsComponent();
        shared_ptr<GraphicsComponent> getGraphicsComponent();
        shared_ptr<NanoVGComponent> getNanoVGComponent();
        shared_ptr<IWindowComponent> getWindowComponent();
        shared_ptr<LuaComponent> getLuaComponent();
        shared_ptr<Project> getProject();
        shared_ptr<InputComponent> getInputComponent();

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

        shared_ptr<SceneRuntime> constructActiveSceneRuntime
        (shared_ptr<SceneDefinition> sceneDefinition);

        bool hasActiveSceneRuntime();
        shared_ptr<SceneRuntime> getActiveSceneRuntime();
        void resetActiveSceneRuntime();

        void useDefinition(shared_ptr<IDefinition>) override;

        shared_ptr<FontCache> getFontCache();
        shared_ptr<ShaderCache> getShaderCache();
        shared_ptr<MaterialCache> getTextureCache();
        shared_ptr<AssimpCache> getModelCache();
        bool mGraphicsUpdating;
        bool mLogicUpdating;

        bool getLogicUpdating() const;
        bool getGraphicsUpdating() const;

        void cleanUpThreads();
        bool getParallel() const;
        void setParallel(bool parallel);

    private: // Member Functions
        bool initPathComponent();
        bool initAudioComponent();
        bool initInputComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initWindowComponent();
        bool initLuaComponent();
        bool initCaches();

        void cleanUpPathComponentThread();
        void cleanUpAudioComponentThread();
        void cleanUpInputComponentThread();
        void cleanUpGraphicsComponentThread();
        void cleanUpLuaComponentThread();
        void cleanUpPhysicsComponentThread();
    };

} // End Dream
