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
#include <luabind/luabind.hpp>

extern "C" {
    #include "lualib.h"
}

#include "../../DreamEngine.h"
#include "../../Constants.h"

#define LUA_SCRIPT_ON_INIT_FUNCTION   "onInit"
#define LUA_SCRIPT_ON_UPDATE_FUNCTION "onUpdate"
#define LUA_SCRIPT_ON_INPUT_FUNCTION  "onInput"
#define LUA_SCRIPT_ON_EVENT_FUNCTION  "onEvent"

namespace Dream {
  using namespace std;

  class LuaComponent {
  private:
    string mScriptLoadFromFile =
        "function scriptLoadFromFile (scriptTable, script_path)\n"
        "    local mt = {__index = _G}\n"
        "    setmetatable(scriptTable, mt)\n"
        "    local chunk = loadfile(script_path)\n"
        "    setfenv(chunk, scriptTable)\n"
        "    chunk()\n"
        "end";
    string mScriptLoadFromString =
        "function scriptLoadFromString (scriptTable, script_string)\n"
        "    local mt = {__index = _G}\n"
        "    setmetatable(scriptTable, mt)\n"
        "    local chunk = loadstring(script_string)\n"
        "    setfenv(chunk, scriptTable)\n"
        "    chunk()\n"
        "end";
    lua_State *mState;
    map<SceneObject*, LuaScriptInstance*> *mScriptMap;
    vector<SDL_Event> mSDLEvents;
  public:
    LuaComponent();
    ~LuaComponent();

    void setLuaScriptMap(map<SceneObject*,LuaScriptInstance*>*);

    void bindDreamEngine();
    void bindMath();
    void bindComponents();
    void bindProject();
    void bindSceneObject();
    void bindScene();
    void bindTime();
    void bindTransform3D();
    void bindAnimationComponent();
    void bindSDLAudioComponent();
    void bindIWindowComponent();
    void bindGraphicsComponent();
    void bindPhysicsComponent();
    void bindSDL();
    void bindSDL_Event();
    void bindCamera();
    void bindGameController();

    void bindAssetClasses();
    void bindSDLAudioInstance();
    void bindFontInstance();
    void bindAnimationInstance();
    void bindAssimpModelInstance();
    void bindLightInstance();
    void bindShaderInstance();
    void bindSpriteInstance();
    void bindPhysicsObjectInstance();
    void bindDreamEvent();

    bool init();
    bool createAllScripts();
    bool loadScript(SceneObject*, LuaScriptInstance*);
    bool update();
    void setSDL_Events(vector<SDL_Event>);
    void stackDump();

    bool executeScriptInit(SceneObject*);
    bool executeScriptUpdate(SceneObject*);
    bool executeScriptKeyHandler(SceneObject*);
    bool executeScriptEventHandler(SceneObject*);

  }; // End of LuaComponent
} // End of Dream

int errorHandler(lua_State *L);

#endif // LUACOMPONENT_H
