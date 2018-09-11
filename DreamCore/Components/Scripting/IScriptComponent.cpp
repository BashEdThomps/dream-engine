/*
 * IScriptComponent
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

#include "IScriptComponent.h"

#include <sstream>

#include "ScriptInstance.h"

#include "../Event.h"
#include "../Transform3D.h"
#include "../Time.h"
#include "../Path/PathComponent.h"
#include "../Path/PathInstance.h"
#include "../Audio/AudioComponent.h"
#include "../Audio/AudioInstance.h"
#include "../Graphics/Model/AssimpModelInstance.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Font/FontInstance.h"
#include "../Graphics/GraphicsComponent.h"
#include "../Graphics/NanoVGComponent.h"
#include "../Graphics/Light/LightInstance.h"
#include "../Graphics/Shader/ShaderInstance.h"
#include "../Graphics/Sprite/SpriteInstance.h"
#include "../Input/InputComponent.h"
#include "../Physics/PhysicsComponent.h"
#include "../Physics/PhysicsObjectInstance.h"
#include "../Window/IWindowComponent.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Utilities/Math.h"
#include "../../../NanoVG/src/nanovg.h"


namespace Dream
{
    IScriptComponent::IScriptComponent
    (shared_ptr<ProjectRuntime> project, shared_ptr<ScriptCache> cache)
        : IComponent(),
          mScriptCache(cache),
          mProjectRuntime(project)
    {
        setLogClassName("IScriptComponent");
        auto log = getLog();
        log->trace( "Constructing Object" );
    }

    IScriptComponent::~IScriptComponent
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );
        mScriptMap.clear();
    }

    bool
    IScriptComponent::createScript
    (shared_ptr<SceneObjectRuntime> sceneObject, shared_ptr<ScriptInstance> script)
    {
        auto log = getLog();
        if (script == nullptr)
        {
            log->error( "Load Failed, ScriptInstance is NULL" );
            return false;
        }

        if (sceneObject == nullptr)
        {
            log->error( "Load Failed, SceneObjectRuntime is NULL" );
            return false;
        }

        if (script->getLoadedFlag())
        {
            log->info( "Script {} is already loaded" , script->getNameAndUuidString());
            return false;
        }

        log->info( "Loading script '{}' for '{}'" , script->getName(),sceneObject->getName());
        log->info( "Loading script from {}" , script->getAbsolutePath());

        if (!loadScript(sceneObject))
        {
            return false;
        }

        log->info( "Loaded {} successfully" , sceneObject->getUuid());

        script->setLoadedFlag(true);
        executeScriptInit(sceneObject);

        return true;
    }

    void
    IScriptComponent::updateComponent
    ()
    {
            beginUpdate();
            auto log = getLog();
            log->info( "Update Called" );

            for (auto entry : mScriptMap)
            {
                auto key = entry.first;

                if (!executeScriptUpdate(key))
                {
                    return;
                }

                if (key->hasFocus())
                {
                    if (!executeScriptInput(key))
                    {
                        return;
                    }
                }

                if (key->hasEvents())
                {
                    if (!executeScriptEvent(key))
                    {
                        return;
                    }
                }
            }
            endUpdate();
    }

    bool
    IScriptComponent::updateNanoVG
    ()
    {
        auto log = getLog();
        log->info( "UpdateNanoVG Called" );

        for (auto entry : mScriptMap)
        {
            shared_ptr<SceneObjectRuntime> key = entry.first;

            if (!executeScriptNanoVG(key))
            {
                return false;
            }
        }
        return true;
    }

    void
    IScriptComponent::setInputMap
    (shared_ptr<gainput::InputMap> map)
    {
        mInputMap = map;
    }

    // API Exposure Methods ======================================================

    void
    IScriptComponent::debugRegisteringClass
    (string className)
    {
        auto log = getLog();
        log->info( "Registering Class {}",  className );
        return;
    }

    void
    IScriptComponent::exposeAPI
    ()
    {
        // Dream Base
        exposeDreamBase();
        // Runtimes
        exposeProjectRuntime();
        exposeSceneObjectRuntime();
        // Dream Misc
        exposeEvent();
        exposeMath();
        exposeTime();
        exposeTransform3D();
        // Audio
        exposeAudioComponent();
        exposeAudioInstance();
        // Graphics
        exposeGraphicsComponent();
        exposeAssimpModelInstance();
        exposeCamera();
        exposeFontInstance();
        exposeLightInstance();
        exposeShaderInstance();
        exposeSpriteInstance();
        exposeNanoVG();
        exposeGLM();
        // Input
        exposeGainput();
        // Path
        exposePathComponent();
        exposePathInstance();
        // Script
        exposeScriptInstance();
        // Physics
        exposePhysicsComponent();
        exposePhysicsObjectInstance();
    }

} // End of Dream

