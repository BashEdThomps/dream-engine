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
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
};

#include <map>
#include <iostream>

#include "InputEvent.h"

#include "../Common/Constants.h"

using namespace std;

int errorHandler(lua_State*);

namespace Dream
{
    class Project;
    class Scene;
    class SceneObject;
    class LuaScriptInstance;
    class Event;

    class LuaEngine
    {
    public: // Methods
        LuaEngine(Project* projectHandle);
        virtual ~LuaEngine();
        bool init();
        bool createAllScripts();
        bool loadScript(SceneObject*);
        bool update();
        void stackDump();
        void cleanUp(Scene*);
        bool executeScriptInit(SceneObject*);
        bool executeScriptUpdate(SceneObject*);
        bool executeScriptInputHandler(SceneObject*);
        bool executeScriptEventHandler(SceneObject*);
        void removeFromScriptMap(SceneObject*);
        void addToScriptMap(SceneObject*,LuaScriptInstance*);
        void addInputEvent(InputEvent event);
        void clearInputEvents();

    private:// Variables
        Project* mProjectHandle;
        string mScriptLoadFromString =
                "function scriptLoadFromString (scriptTable, script_string)\n"
                "    local mt = {__index = _G}\n"
                "    setmetatable(scriptTable, mt)\n"
                "    local chunk = load(script_string,nil,nil,scriptTable)\n"
                "    --setfenv(chunk, scriptTable)\n"
                "    chunk()\n"
                "end";
        lua_State *mState;
        map<SceneObject*, LuaScriptInstance*> mScriptMap;
        vector<InputEvent> mInputEvents;

    private: // Methods
        // API Exposure Methods ======================================================

        void debugRegisteringClass(string className);
        void exposeAPI();
        void exposeAnimationComponent();
        void exposeAnimationInstance();
        void exposeAssimpModelInstance();
        void exposeCamera();
        void exposeProject();
        void exposeProjectRuntime();
        void exposeEvent();
        void exposeFontInstance();
        void exposeGraphicsComponent();
        void exposeInputEvent();
        void exposeAudioComponent();
        void exposeAudioInstance();
        void exposeIAssetInstance();
        void exposeIWindowComponent();
        void exposeLightInstance();
        void exposeLuaScriptInstance();
        void exposeMath();
        void exposePhysicsComponent();
        void exposePhysicsObjectInstance();
        void exposeShaderInstance();
        void exposeSpriteInstance();
        void exposeScene();
        void exposeSceneObject();
        void exposeTime();
        void exposeTransform3D();
    }; // End of LuaComponent

} // End of Dream
