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
};

#include <map>
#include <iostream>
#include <memory>

#include "LuaScriptCache.h"
#include "../../Common/Constants.h"
#include "../IComponent.h"
#include <gainput/gainput.h>

using std::unique_ptr;
using std::string;
using std::map;
using std::vector;

int errorr(lua_State*);

namespace Dream
{
    class ProjectRuntime;
    class SceneRuntime;
    class SceneObjectRuntime;
    class LuaScriptInstance;
    class Event;

    class LuaComponent : public IComponent
    {
    public: // Methods
        LuaComponent(
            shared_ptr<ProjectRuntime> project,
            shared_ptr<LuaScriptCache> cache
        );

       ~LuaComponent() override;

        bool init() override;
        void updateComponent() override;
        bool createScript(shared_ptr<SceneObjectRuntime>,shared_ptr<LuaScriptInstance>);
        bool loadScript(shared_ptr<SceneObjectRuntime>);
        bool updateNanoVG();
        void stackDump();

        bool executeScriptInit  (shared_ptr<SceneObjectRuntime>);
        bool executeScriptUpdate(shared_ptr<SceneObjectRuntime>);
        bool executeScriptNanoVG(shared_ptr<SceneObjectRuntime>);
        bool executeScriptInput (shared_ptr<SceneObjectRuntime>);
        bool executeScriptEvent (shared_ptr<SceneObjectRuntime>);

        void removeFromScriptMap(shared_ptr<SceneObjectRuntime>);
        void addToScriptMap(shared_ptr<SceneObjectRuntime>,shared_ptr<LuaScriptInstance>);

        void setInputMap(gainput::InputMap *map);

    private:// Variables
        shared_ptr<LuaScriptCache> mScriptCache;
        shared_ptr<ProjectRuntime> mProjectRuntime;
        string mScriptLoadFromString =
                "function scriptLoadFromString (scriptTable, script_string)\n"
                "    local mt = {__index = _G}\n"
                "    setmetatable(scriptTable, mt)\n"
                "    local chunk = assert(loadstring(script_string))\n"
                "    if not chunk then\n "
                "       return false\n"
                "    end\n"
                "    setfenv(chunk, scriptTable)"
                "    chunk()\n"
                "    return true\n"
                "end";

                /*"function scriptLoadFromString (scriptTable, script_string)\n"
                "    local mt = {__index = _G}\n"
                "    setmetatable(scriptTable, mt)\n"
                "    local chunk = assert(load(script_string,nil,nil,scriptTable))\n"
                "    if not chunk then return false end\n"
                "    chunk()\n"
                "    return true\n"
                "end";
                        */
        lua_State *mState;
        map<shared_ptr<SceneObjectRuntime>, shared_ptr<LuaScriptInstance>> mScriptMap;
        gainput::InputMap* mInputMap;

    private: // Methods
        // API Exposure Methods ======================================================

        void debugRegisteringClass(string className);
        void exposeAPI();
        void exposeDreamBase();
        void exposePathComponent();
        void exposePathInstance();
        void exposeAssimpModelInstance();
        void exposeCamera();
        void exposeProjectRuntime();
        void exposeEvent();
        void exposeFontInstance();
        void exposeGraphicsComponent();
        void exposeGainput();
        void exposeAudioComponent();
        void exposeAudioInstance();
        void exposeIWindowComponent();
        void exposeLightInstance();
        void exposeLuaScriptInstance();
        void exposeMath();
        void exposePhysicsComponent();
        void exposePhysicsObjectInstance();
        void exposeShaderInstance();
        void exposeSpriteInstance();
        void exposeSceneObjectRuntime();
        void exposeTime();
        void exposeTransform3D();
        void exposeNanoVG();
        void exposeGLM();
    }; // End of LuaComponent

} // End of Dream
