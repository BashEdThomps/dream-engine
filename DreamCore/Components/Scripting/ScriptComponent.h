/*
 * LuaComponent.h
 *
 * Created: 26/11/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
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

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include "ScriptInstance.h"
#include "ScriptCache.h"
#include "../IComponent.h"

namespace Dream
{
    class ProjectRuntime;
    class SceneRuntime;
    class SceneObjectRuntime;
    class Event;
    class ScriptCache;

    class LuaPrintListener
    {
    public:
        virtual ~LuaPrintListener();
        virtual void onPrint(string) = 0;
    };

    class ScriptComponent : public IComponent
    {
    public: // Methods

        static vector<LuaPrintListener*> PrintListeners;
        static void AddPrintListener(LuaPrintListener* listener);

        ScriptComponent(ProjectRuntime* runtime, ScriptCache* cache);
       ~ScriptComponent();

        bool init();
        void updateComponent(SceneRuntime*);
        bool updateNanoVG();
        bool loadScript(SceneObjectRuntime*);
        bool createScript(SceneObjectRuntime*, ScriptInstance*);
        void exposeAPI();
        void debugRegisteringClass(string);

        static lua_State* State;
    private:
        ScriptCache* mScriptCache;
        ProjectRuntime* mProjectRuntime;

        // API Exposure Methods ======================================================
        void exposePathComponent();
        void exposePathInstance();
        void exposeModelInstance();
        void exposeCamera();
        void exposeProjectRuntime();
        void exposeEvent();
        void exposeGraphicsComponent();
        void exposeInputComponent();
        void exposeAudioComponent();
        void exposeAudioInstance();
        void exposeLightInstance();
        void exposeScriptInstance();
        void exposePhysicsComponent();
        void exposePhysicsObjectInstance();
        void exposeShaderInstance();
        void exposeSceneRuntime();
        void exposeSceneObjectRuntime();
        void exposeTime();
        void exposeTransform();
        void exposeGLM();
        void exposeIDefinition();
        void exposeNanoVG();

    }; // End of LuaComponent
} // End of Dream
