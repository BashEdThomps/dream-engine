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
#include <memory>

#include "LuaScriptCache.h"
#include "../../Common/Constants.h"
#include "../IComponent.h"
#include <gainput/gainput.h>

using std::unique_ptr;
using std::string;
using std::map;
using std::vector;

int errorHandler(lua_State*);

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
        LuaComponent(ProjectRuntime* projectHandle, LuaScriptCache* cache);
        virtual ~LuaComponent();

        bool init() override;
        void updateComponent() override;
        bool createScript(SceneObjectRuntime*,LuaScriptInstance*);
        bool loadScript(SceneObjectRuntime*);
        bool updateNanoVG();
        void stackDump();

        bool executeScriptInit(SceneObjectRuntime*);
        bool executeScriptUpdate(SceneObjectRuntime*);
        bool executeScriptNanoVG(SceneObjectRuntime*);
        bool executeScriptInputHandler(SceneObjectRuntime*);
        bool executeScriptEventHandler(SceneObjectRuntime*);

        void removeFromScriptMap(SceneObjectRuntime*);
        void addToScriptMap(SceneObjectRuntime*,LuaScriptInstance*);

        void setInputMap(gainput::InputMap *map);

    private:// Variables
        LuaScriptCache* mScriptCacheHandle;
        ProjectRuntime* mProjectRuntimeHandle;
        string mScriptLoadFromString =
                "function scriptLoadFromString (scriptTable, script_string)\n"
                "    local mt = {__index = _G}\n"
                "    setmetatable(scriptTable, mt)\n"
                "    local chunk = assert(load(script_string,nil,nil,scriptTable))\n"
                "    if not chunk then return false end\n"
                "    chunk()\n"
                "    return true\n"
                "end";
        lua_State *mState;
        map<SceneObjectRuntime*, LuaScriptInstance*> mScriptMap;
        gainput::InputMap* mInputMap;

    private: // Methods
        // API Exposure Methods ======================================================

        void debugRegisteringClass(string className);
        void exposeAPI();
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
        void exposeIAssetInstance();
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
