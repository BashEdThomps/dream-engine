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
#ifndef LUACOMPONENT_H
#define LUACOMPONENT_H

#include <map>
#include <iostream>


extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include "../Constants.h"
#include "../Event.h"
#include "../SceneObject.h"

#define LUA_SCRIPT_ON_INIT_FUNCTION   "onInit"
#define LUA_SCRIPT_ON_UPDATE_FUNCTION "onUpdate"
#define LUA_SCRIPT_ON_INPUT_FUNCTION  "onInput"
#define LUA_SCRIPT_ON_EVENT_FUNCTION  "onEvent"

using namespace std;

int errorHandler(lua_State*);

namespace Dream
{
    class DreamEngine;
    class LuaEngine
    {
    public: // Methods
        LuaEngine(DreamEngine* engine);
        virtual ~LuaEngine();
        void setLuaScriptMap(map<SceneObject*,LuaScriptInstance*>*);
        bool init();
        bool createAllScripts();
        bool loadScript(SceneObject*);
        bool update();
        void stackDump();
        void cleanUp();
        bool executeScriptInit(SceneObject*);
        bool executeScriptUpdate(SceneObject*);
        bool executeScriptKeyHandler(SceneObject*);
        bool executeScriptEventHandler(SceneObject*);

    private:// Variables
        DreamEngine* mDreamEngine;
        /*
        string mScriptLoadFromStringOld =
                "function scriptLoadFromString (scriptTable, script_string)\n"
                "    local mt = {__index = _G}\n"
                "    setmetatable(scriptTable, mt)\n"
                "    local chunk = load(script_string)\n"
                "    setfenv(chunk, scriptTable)\n"
                "    chunk()\n"
                "end";
                */

        string mScriptLoadFromString =
                "function scriptLoadFromString (scriptTable, script_string)\n"
                "    local mt = {__index = _G}\n"
                "    setmetatable(scriptTable, mt)\n"
                "    local chunk = load(script_string,nil,nil,scriptTable)\n"
                "    --setfenv(chunk, scriptTable)\n"
                "    chunk()\n"
                "end";
        lua_State *mState;
        map<SceneObject*, LuaScriptInstance*> *mScriptMap;

    private: // Methods
        // API Exposure Methods ======================================================

        void debugRegisteringClass(string className);
        void exposeAPI();
        void exposeAnimationComponent();
        void exposeAnimationInstance();
        void exposeAssimpModelInstance();
        void exposeCamera();
        void exposeDreamEngine();
        void exposeEvent();
        void exposeFontInstance();
        void exposeGameController();
        void exposeGraphicsComponent();
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

#endif // LUACOMPONENT_H
