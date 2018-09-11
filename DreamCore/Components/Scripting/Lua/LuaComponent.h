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

#include <map>
#include <iostream>
#include <memory>

#include "../ScriptCache.h"
#include "../../../Common/Constants.h"
#include "../IScriptComponent.h"
#include <gainput/gainput.h>

#include "LuaScriptInstance.h"

using std::unique_ptr;
using std::string;
using std::map;
using std::vector;

namespace Dream
{
    class ProjectRuntime;
    class SceneRuntime;
    class SceneObjectRuntime;
    class Event;

    class LuaComponent : public IScriptComponent
    {
    public: // Methods
        LuaComponent(shared_ptr<ProjectRuntime> project, shared_ptr<ScriptCache> cache);
       ~LuaComponent() override;

        bool init() override;
        void updateComponent() override;
        bool loadScript(shared_ptr<SceneObjectRuntime>) override;
        bool updateNanoVG();

        bool executeScriptInit  (shared_ptr<SceneObjectRuntime>) override;
        bool executeScriptUpdate(shared_ptr<SceneObjectRuntime>) override;
        bool executeScriptNanoVG(shared_ptr<SceneObjectRuntime>) override;
        bool executeScriptInput (shared_ptr<SceneObjectRuntime>) override;
        bool executeScriptEvent (shared_ptr<SceneObjectRuntime>) override;

        void removeFromScriptMap(shared_ptr<SceneObjectRuntime>) override;
        void addToScriptMap(shared_ptr<SceneObjectRuntime>, shared_ptr<ScriptInstance>) override;

    private:
        lua_State* mState;

        // API Exposure Methods ======================================================

        void exposePathComponent() override;
        void exposePathInstance() override;
        void exposeAssimpModelInstance() override;
        void exposeCamera() override;
        void exposeProjectRuntime() override;
        void exposeEvent() override;
        void exposeFontInstance() override;
        void exposeGraphicsComponent() override;
        void exposeGainput() override;
        void exposeAudioComponent() override;
        void exposeAudioInstance() override;
        void exposeLightInstance() override;
        void exposeScriptInstance() override;
        void exposePhysicsComponent() override;
        void exposePhysicsObjectInstance() override;
        void exposeShaderInstance() override;
        void exposeSpriteInstance() override;
        void exposeSceneObjectRuntime() override;
        void exposeTime() override;
        void exposeTransform3D() override;
        void exposeNanoVG() override;
        void exposeGLM() override;
    }; // End of LuaComponent

} // End of Dream
