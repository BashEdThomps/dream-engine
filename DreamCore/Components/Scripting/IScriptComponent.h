/*
 * IScriptComponent.h
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

#include <map>
#include <iostream>
#include <memory>

#include "ScriptCache.h"
#include "../../Common/Constants.h"
#include "../IComponent.h"
#include <gainput/gainput.h>

using std::unique_ptr;
using std::string;
using std::map;
using std::vector;

namespace Dream
{
    class ProjectRuntime;
    class SceneRuntime;
    class SceneObjectRuntime;
    class ScriptInstance;
    class Event;

    class IScriptComponent : public IComponent
    {
    public: // Methods
        IScriptComponent(
            ProjectRuntime* project,
            ScriptCache* cache
        );

       virtual ~IScriptComponent() override;

        virtual bool createScript(SceneObjectRuntime*,ScriptInstance*) = 0;
        virtual bool loadScript(SceneObjectRuntime*) = 0;
        virtual bool updateNanoVG() = 0;

        virtual bool executeScriptInit  (SceneObjectRuntime*) = 0;
        virtual bool executeScriptUpdate(SceneObjectRuntime*) = 0;
        virtual bool executeScriptInput (SceneObjectRuntime*) = 0;
        virtual bool executeScriptEvent (SceneObjectRuntime*) = 0;
        virtual bool executeScriptNanoVG(SceneObjectRuntime*) = 0;

        virtual void removeFromScriptMap(SceneObjectRuntime*) = 0;
        virtual void addToScriptMap(SceneObjectRuntime*,ScriptInstance*) = 0;
        virtual void setInputMap(gainput::InputMap* map) = 0;

    protected:// Variables
        ScriptCache* mScriptCache;
        ProjectRuntime* mProjectRuntime;
        map<SceneObjectRuntime*, ScriptInstance*> mScriptMap;
        gainput::InputMap* mInputMap;

        // API Exposure Methods ======================================================
        virtual void debugRegisteringClass(string className) = 0;
        virtual void exposeAPI() = 0;
        virtual void exposePathComponent() = 0;
        virtual void exposePathInstance() = 0;
        virtual void exposeAssimpModelInstance() = 0;
        virtual void exposeCamera() = 0;
        virtual void exposeProjectRuntime() = 0;
        virtual void exposeEvent() = 0;
        virtual void exposeGraphicsComponent() = 0;
        virtual void exposeGainput() = 0;
        virtual void exposeAudioComponent() = 0;
        virtual void exposeAudioInstance() = 0;
        virtual void exposeLightInstance() = 0;
        virtual void exposeScriptInstance() = 0;
        virtual void exposePhysicsComponent() = 0;
        virtual void exposePhysicsObjectInstance() = 0;
        virtual void exposeShaderInstance() = 0;
        virtual void exposeSceneObjectRuntime() = 0;
        virtual void exposeTime() = 0;
        virtual void exposeTransform3D() = 0;
        virtual void exposeGLM() = 0;
        virtual void exposeIDefinition() = 0;
        virtual void exposeNanoVG() = 0;

    }; // End of IScriptComponent

} // End of Dream
