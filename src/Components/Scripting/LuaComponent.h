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
#include "../../Constants.h"

extern "C" {
    #include "lualib.h"
}

#include "../../DreamEngine.h"

#define LUA_SCRIPT_UPDATE_FUNCTION "update"
#define LUA_SCRIPT_HANDLE_INPUT_FUNCTION "handleInput"

namespace Dream {
  using namespace std;

  class LuaComponent {
  private:
    string mScriptLoaderCode = "function scriptloader (scriptTable, scriptname)\n"
                               "    local mt = {__index = _G}\n"
                               "    setmetatable(scriptTable, mt)\n"
                               "    local chunk = loadfile(scriptname)\n"
                               "    setfenv(chunk, scriptTable)\n"
                               "    chunk()\n"
                               "end";
    lua_State *mState;
    map<SceneObject*, LuaScriptInstance*> *mScriptMap;
    SDL_Event mEvent;
  public:
    LuaComponent();
    ~LuaComponent();

    void setLuaScriptMap(map<SceneObject*,LuaScriptInstance*>*);

    void bindDreamEngine();
    void bindAssetManager();
    void bindComponents();
    void bindProject();
    void bindSceneObject();
    void bindScene();
    void bindTime();
    void bindTransform3D();
    void bindAnimationComponent();
    void bindAudioComponent();
    void bindGraphicsComponent();
    void bindPhysicsComponent();
    void bindSDL();
    void bindSDL_Event();

    bool init();

    bool loadScriptsFromMap();
    bool loadScript(SceneObject*, LuaScriptInstance*);
    void stackDump();
    bool update();
    bool executeScriptUpdate(SceneObject*, LuaScriptInstance*);
    bool executeScriptKeyHandler(SceneObject*, LuaScriptInstance*);
    void setSDL_Event(SDL_Event);
    void bindAssetClasses();
    void bindAudioInstance();
    /*
    void bindAnimationInstance();
    void bindAssimpModelInstance();
    void bindLightInstance();
    void bindShaderInstance();
    void bindSpriteInstance();
    void bindPhysicsObjectInstane();
    */
    void bindFontInstance();


  }; // End of LuaComponent
} // End of Dream

int errorHandler(lua_State *L);

#endif // LUACOMPONENT_H
