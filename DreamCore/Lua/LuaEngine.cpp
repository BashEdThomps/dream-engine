/*
 * LuaEngine
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

#include "LuaEngine.h"

#include <sstream>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

#include "LuaScriptInstance.h"

#include "../Components/Event.h"
#include "../Components/Transform3D.h"
#include "../Components/Time.h"
#include "../Components/Animation/AnimationComponent.h"
#include "../Components/Animation/AnimationInstance.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Audio/AudioInstance.h"
#include "../Components/Graphics/Model/AssimpModelInstance.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/Font/FontInstance.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/NanoVGComponent.h"
#include "../Components/Graphics/Light/LightInstance.h"
#include "../Components/Graphics/Shader/ShaderInstance.h"
#include "../Components/Graphics/Sprite/SpriteInstance.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Physics/PhysicsObjectInstance.h"
#include "../Components/Window/IWindowComponent.h"
#include "../Project/ProjectRuntime.h"
#include "../Scene/SceneRuntime.h"
#include "../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Utilities/Math.h"
#include "../../NanoVG/src/nanovg.h"

using std::ostringstream;
using std::exception;
using std::cout;
using std::cerr;
using std::string;

using luabind::object;
using luabind::from_stack;
using luabind::call_function;
using luabind::globals;
using luabind::class_;
using luabind::constructor;
using luabind::open;
using luabind::set_pcall_callback;
using luabind::registry;
using luabind::error;
using luabind::module;
using luabind::value;
using luabind::nil;
using luabind::newtable;
using luabind::def;

int
errorHandler
(lua_State *L)
{
    auto log = spdlog::get("LuaErrorHandler");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("LuaErrorHandler");
    }
    // log the error message
    object msg(from_stack( L, -1 ));
    stringstream ss;
    ss << msg;
    log->error("RuntimeError: {}",ss.str());
    // log the callstack
    string traceback = call_function<string>( globals(L)["debug"]["traceback"] );
    log->error("TraceBack: {}", traceback.c_str());
    return 0;
}

namespace Dream
{
    LuaEngine::LuaEngine
    (ProjectRuntime* projectHandle, LuaScriptCache* cache)
        : ILoggable("LuaEngine"),
          mScriptCacheHandle(cache),
          mProjectRuntimeHandle(projectHandle)
    {
        auto log = getLog();
        log->info( "Constructing Object" );
    }

    LuaEngine::~LuaEngine
    ()
    {
        auto log = getLog();
        log->info( "Destroying Object" );

        if (mState != nullptr)
        {
            lua_close(mState);
        }

        mScriptMap.clear();
    }

    bool
    LuaEngine::init
    ()
    {
        auto log = getLog();
            log->info( "Initialising LuaEngine" );
        mState = luaL_newstate();
        if (mState)
        {
            open(mState);
            luaopen_base(mState);
            set_pcall_callback(&errorHandler);
            luaL_dostring(mState, mScriptLoadFromString.c_str());
            exposeAPI();
            return true;
        }
        else
        {
            log->error( "Error creating lua state, LuaEngine::mState == nullptr");
        }
        return false;
    }

    bool
    LuaEngine::createScript
    (SceneObjectRuntime* sceneObject, LuaScriptInstance* luaScript)
    {
        auto log = getLog();
        if (luaScript == nullptr)
        {
            log->error( "Load Failed, LuaScriptInstance is NULL" );
            return false;
        }

        if (sceneObject == nullptr)
        {
            log->error( "Load Failed, SceneObjectRuntime is NULL" );
            return false;
        }

        if (luaScript->getLoadedFlag())
        {
            log->info( "Script {} is already loaded" , luaScript->getNameAndUuidString());
            return false;
        }

            log->info( "Loading script '{}' for '{}'" , luaScript->getName(),sceneObject->getName());
            log->info( "Loading Lua script from {}" , luaScript->getAbsolutePath());

        if (!loadScript(sceneObject))
        {
            return false;
        }

        log->info( "Loaded {} successfully" , sceneObject->getUuid());

        luaScript->setLoadedFlag(true);
        executeScriptInit(sceneObject);

        return true;
    }

    bool
    LuaEngine::loadScript
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (!mState)
        {
            log->error( "Cannot load script, mState == nullptr!" );
            return false;
        }

        if (scriptInstance->getError())
        {
           log->error( "Cannot load script {} while in error state",id );
           return false;
        }

        log->info( "loadScript called for {}", id );

        try
        {
            log->info( "Creating new table for {}" ,id );

            object newScriptTable = newtable(mState);
            string path = scriptInstance->getAbsolutePath();
            string script = mScriptCacheHandle->getScript(path);

            log->info( "calling scriptLoadFromString in lua for {}" , id );

            call_function<void>(mState, "scriptLoadFromString", newScriptTable, script.c_str());

            object reg = registry(mState);
            reg[id] = newScriptTable;
        }
        catch (error &e)
        {
            log->error("loadScript exception: {}" , e.what() );
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    void
    LuaEngine::stackDump
    ()
    {
        auto log = getLog();
        log->error( "Stack Dump!" );
        int i;
        int top = lua_gettop(mState);
        // repeat for each level
        for (i = 1; i <= top; i++)
        {
            int t = lua_type(mState, i);
            switch (t)
            {
                // strings
                case LUA_TSTRING:
                    log->error( lua_tostring(mState, i));
                    break;
                    // booleans
                case LUA_TBOOLEAN:
                    log->error( (lua_toboolean(mState, i) ? "true" : "false"));
                    break;
                    // numbers
                case LUA_TNUMBER:
                    log->error( lua_tonumber(mState, i));
                    break;
                    // other values
                default:
                    log->info( lua_typename(mState, t));
                    break;
            }
        }
        // end the listing
    }

    bool
    LuaEngine::update
    ()
    {
        auto log = getLog();
        log->info( "Update Called" );

        for (pair<SceneObjectRuntime*,LuaScriptInstance*> entry : mScriptMap)
        {
            SceneObjectRuntime* key = entry.first;

            if (!executeScriptUpdate(key))
            {
                return false;
            }

            if (key->hasFocus())
            {
                if (!executeScriptInputHandler(key))
                {
                    return false;
                }
            }

            if (key->hasEvents())
            {
                if (!executeScriptEventHandler(key))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool
    LuaEngine::updateNanoVG
    ()
    {
        auto log = getLog();
        log->info( "UpdateNanoVG Called" );

        for (pair<SceneObjectRuntime*,LuaScriptInstance*> entry : mScriptMap)
        {
            SceneObjectRuntime* key = entry.first;

            if (!executeScriptNanoVG(key))
            {
                return false;
            }
        }
        return true;
    }

    bool
    LuaEngine::executeScriptUpdate
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            log->error( "Cannot execute {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info( "Calling onUpdate for {}" ,sceneObject->getNameAndUuidString() );

        try
        {
            object reg = registry(mState);
            object table = reg[id];
            object funq = table[Constants::LUA_UPDATE_FUNCTION];
            if (funq.is_valid())
            {
                call_function<void>(funq,sceneObject);
            }
            else
            {
                log->error( "Attempted to call onUpdate on invalid function.");
            }
        }
        catch (error e)
        {
            string error = lua_tostring( e.state(), -1 );
            log->error( "onUpdate exception: {}" , e.what() );
            log->error( error );
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    bool
    LuaEngine::executeScriptNanoVG
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            log->error( "Cannot execute NanoVG {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info( "Calling onNanoVG for {}" , sceneObject->getNameAndUuidString() );

        try
        {
            object reg = registry(mState);
            object table = reg[id];
            object funq = table[Constants::LUA_NANOVG_FUNCTION];
            if (funq.is_valid())
            {
                call_function<void>(funq,sceneObject);
            }
            else
            {
                log->error( "Attempted to call onNanoVG on invalid function.");
            }
        }
        catch (luabind::error e)
        {
            string error = lua_tostring( e.state(), -1 );
            log->error( "onNanoVG exception: {}" , e.what() );
            log->error( error );
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    bool
    LuaEngine::executeScriptInit
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError() )
        {
            log->error( "Cannot execute {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info( "Calling onInit in {} for {}",  scriptInstance->getName(),  sceneObject->getName());
        try
        {
            object reg = registry(mState);
            object table = reg[id];
            object funq = table[Constants::LUA_INIT_FUNCTION];
            if (funq.is_valid())
            {
                call_function<void>(funq,sceneObject);
            }
            else
            {
                log->error( "Attempted to call onInit on invalid function.");
            }

        }
        catch (error e)
        {
            string error = lua_tostring( e.state(), -1 );
            log->error( "onInit exception: {}" , e.what() );
            log->error( error );
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    bool
    LuaEngine::executeScriptInputHandler
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            log->error( "Cannot execute {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info( "Calling onInput for {} (Has Focus) {}", sceneObject->getNameAndUuidString());

        try
        {
            object reg = registry(mState);
            object table = reg[id];
            object funq = table[Constants::LUA_INPUT_FUNCTION];
            if (funq.is_valid())
            {
                for(InputEvent event : mInputEvents)
                {
                   call_function<void>(funq,sceneObject,event);
                }
            }
            else
            {
                log->error( "Attempted to call onInput on invalid function.");
            }

            clearInputEvents();
        }
        catch (error e)
        {
            string error = lua_tostring( e.state(), -1 );
            log->error( "onInput exception: {}" ,  e.what() );
            log->error( error );
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    bool
    LuaEngine::executeScriptEventHandler
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            log->error( "Cannot execute {} in error state ",  scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info( "Calling onEvent for {}", sceneObject->getNameAndUuidString());

        try
        {
            object reg = registry(mState);
            object table = reg[id];
            object funq = table[Constants::LUA_EVENT_FUNCTION];
            vector<Event> events = sceneObject->getEventQueue();
            if (funq.is_valid())
            {
                for (Event event : events)
                {
                    call_function<void>(funq,sceneObject,event);
                }
                sceneObject->clearEventQueue();
            }
            else
            {
                log->error( "Attempted to call onEvent on invalid function.");
            }
        }
        catch (error e)
        {
            string error = lua_tostring( e.state(), -1 );
            log->error( "onEvent exception: {}", e.what());
            log->error( error );
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    void
    LuaEngine::addInputEvent
    (InputEvent event)
    {
        mInputEvents.push_back(event);
    }

    void
    LuaEngine::clearInputEvents
    ()
    {
        mInputEvents.clear();
    }

    // API Exposure Methods ======================================================

    void
    LuaEngine::exposeProjectRuntime
    ()
    {
        module(mState)
        [
            class_<ProjectRuntime>("ProjectRuntime")
                .def("getAudioComponent",&ProjectRuntime::getAudioComponentHandle)
                .def("getGraphicsComponent",&ProjectRuntime::getGraphicsComponentHandle)
                .def("getNanoVGComponent",&ProjectRuntime::getNanoVGComponentHandle)
                .def("getPhysicsComponent",&ProjectRuntime::getPhysicsComponentHandle)
                .def("getWindowComponent",&ProjectRuntime::getWindowComponentHandle)
                .def("getTime",&ProjectRuntime::getTimeHandle)
                .def("getCamera",&ProjectRuntime::getCameraHandle)
        ];
        globals(mState)["Runtime"] = mProjectRuntimeHandle;
    }

    void
    LuaEngine::exposeCamera
    ()
    {
        module(mState)
                [
                luabind::class_<Camera>("Camera")
                .def("processKeyboard",&Camera::processKeyboard)
                .def("processMouseMovement",&Camera::processMouseMovement)
                .enum_("CameraMovement")
                [
                    value("FORWARD",  Constants::CAMERA_MOVEMENT_FORWARD),
                    value("BACKWARD", Constants::CAMERA_MOVEMENT_BACKWARD),
                    value("LEFT",     Constants::CAMERA_MOVEMENT_LEFT),
                    value("RIGHT",    Constants::CAMERA_MOVEMENT_RIGHT)
                ]
                ];
    }

    void
    LuaEngine::debugRegisteringClass
    (string className)
    {
        auto log = getLog();
        log->info( "Registering Class {}",  className );
        return;
    }

    void
    LuaEngine::exposeAnimationComponent
    ()
    {
        debugRegisteringClass("AnimationComponent");
        module(mState)
                [
                class_<AnimationComponent>("AnimationComponent")
                // TODO
                ];
    }

    void
    LuaEngine::exposeAnimationInstance
    ()
    {
        debugRegisteringClass("AnimationInstance");
        module(mState)
                [
                class_<AnimationInstance>("AnimationInstance")
                // TODO
                ];
    }

    void
    LuaEngine::exposeFontInstance
    ()
    {
        debugRegisteringClass("FontInstance");
        module(mState)
                [
                class_<FontInstance>("FontInstance")
                .def("setText",&FontInstance::setText)
                .def("getText",&FontInstance::getText)
                ];
    }

    void
    LuaEngine::exposeGraphicsComponent
    ()
    {
        debugRegisteringClass("GraphicsComponent");
        module(mState)
                [
                class_<GraphicsComponent>("GraphicsComponent")
                ];
    }

    void
    LuaEngine::exposeLightInstance
    ()
    {
        debugRegisteringClass("LightInstance");
        module(mState)
                [
                class_<LightInstance>("LightInstance")
                // TODO
                ];
    }

    void
    LuaEngine::exposeShaderInstance
    ()
    {
        debugRegisteringClass("ShaderInstance");
        module(mState)
                [
                class_<ShaderInstance>("ShaderInstance")
                .def("getUuid", &ShaderInstance::getUuid)
                .def("setUniform1f", &ShaderInstance::setUniform1f)
                ];
    }

    void
    LuaEngine::exposeSpriteInstance
    ()
    {
        debugRegisteringClass("SpriteInstance");
        module(mState)
                [
                class_<SpriteInstance>("SpriteInstance")
                // TODO
                ];
    }

    void
    LuaEngine::exposePhysicsComponent
    ()
    {
        debugRegisteringClass("PhysicsComponent");
        module(mState)
                [
                class_<PhysicsComponent>("PhysicsComponent")
                .def("setDebug",&PhysicsComponent::setDebug)
                ];
    }

    void
    LuaEngine::exposePhysicsObjectInstance
    ()
    {
        debugRegisteringClass("PhysicsObjectInstance");
        module(mState)
                [
                class_<PhysicsObjectInstance>("PhysicsObjectInstance")
                .def("getUuid", &PhysicsObjectInstance::getUuid)
                .def("setLinearVelocity", &PhysicsObjectInstance::setLinearVelocity)
                ];
    }

    void
    LuaEngine::exposeIWindowComponent
    ()
    {
        debugRegisteringClass("IWindiwComponent");
        module(mState)
                [
                class_<IWindowComponent>("IWindowComponent")
                .def("getWidth",&IWindowComponent::getWidth)
                .def("getHeight",&IWindowComponent::getHeight)
                .def("setShouldClose",&IWindowComponent::setShouldClose)
                .def("getMouseX",&IWindowComponent::getMouseX)
                .def("getMouseY",&IWindowComponent::getMouseY)
                ];
    }

    void
    LuaEngine::exposeMath
    ()
    {
        debugRegisteringClass("Math");
        module(mState)
        [
            class_<Math>("Math")
            .scope[
                luabind::def("degreesToRadians",&Math::degreesToRadians),
                luabind::def("radiansToDegrees",&Math::radiansToDegrees),
                luabind::def("pow",&Math::_pow),
                luabind::def("sinf",&Math::_sinf),
                luabind::def("sqrtf",&Math::_sqrtf)
            ]
        ];
    }

    void
    LuaEngine::exposeSceneObjectRuntime
    ()
    {
        debugRegisteringClass("SceneObjectRuntime");
        module(mState)
        [
            class_<SceneObjectRuntime>("SceneObjectRuntime")

                .def("getChildByUuid",&SceneObjectRuntime::getChildRuntimeHandleByUuid)

                .def("getParent",&SceneObjectRuntime::getParentRuntimeHandle)
                .def("setParent",&SceneObjectRuntime::setParentRuntimeHandle)

                .def("getTransform",&SceneObjectRuntime::getTransform)
                .def("setTransform",&SceneObjectRuntime::setTransform)

                .def("addAssetDefinitionUuidToLoad",&SceneObjectRuntime::addAssetDefinitionUuidToLoad)

                .def("getAnimation",&SceneObjectRuntime::getAnimationInstance)
                .def("getAudio",&SceneObjectRuntime::getAudioInstance)
                .def("getSprite",&SceneObjectRuntime::getSpriteInstance)
                .def("getModel",&SceneObjectRuntime::getModelInstance)
                .def("getShader",&SceneObjectRuntime::getShaderInstance)
                .def("getLight",&SceneObjectRuntime::getLightInstance)
                .def("getFont",&SceneObjectRuntime::getFontInstance)
                .def("getPhysicsObject",&SceneObjectRuntime::getPhysicsObjectInstance)

                .def("hasAnimation",&SceneObjectRuntime::hasAnimationInstance)
                .def("hasAudio",&SceneObjectRuntime::hasAudioInstance)
                .def("hasSprite",&SceneObjectRuntime::hasSpriteInstance)
                .def("hasModel",&SceneObjectRuntime::hasModelInstance)
                .def("hasShader",&SceneObjectRuntime::hasShaderInstance)
                .def("hasLight",&SceneObjectRuntime::hasLightInstance)
                .def("hasFont",&SceneObjectRuntime::hasFontInstance)
                .def("hasPhysicsObject",&SceneObjectRuntime::hasPhysicsObjectInstance)

            ];
    }

    void
    LuaEngine::exposeTransform3D
    ()
    {
        debugRegisteringClass("Transform3D");
        module(mState) [
            class_<Transform3D>("Transform3D")
                .def(constructor<>())
                // Translation ===========================================================
                .def("getTransformType",&Transform3D::getTransformType)
                .def("getTranslationX",&Transform3D::getTranslationX)
                .def("getTranslationY",&Transform3D::getTranslationY)
                .def("getTranslationZ",&Transform3D::getTranslationZ)
                .def("setTranslationX",&Transform3D::setTranslationX)
                .def("setTranslationY",&Transform3D::setTranslationY)
                .def("setTranslationZ",&Transform3D::setTranslationZ)
                .def("translateByX",&Transform3D::translateByX)
                .def("translateByY",&Transform3D::translateByY)
                .def("translateByZ",&Transform3D::translateByZ)
                .def("setTranslation",static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setTranslation))
                // Rotation =============================================================
                .def("getRotationX",&Transform3D::getRotationX)
                .def("getRotationY",&Transform3D::getRotationY)
                .def("getRotationZ",&Transform3D::getRotationZ)
                .def("setRotationX",&Transform3D::setRotationX)
                .def("setRotationY",&Transform3D::setRotationY)
                .def("setRotationZ",&Transform3D::setRotationZ)
                .def("rotateByX",&Transform3D::rotateByX)
                .def("rotateByY",&Transform3D::rotateByY)
                .def("rotateByZ",&Transform3D::rotateByZ)
                .def("setRotation",static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setRotation))
                // Scale ================================================================
                .def("getScaleX",&Transform3D::getScaleX)
                .def("getScaleY",&Transform3D::getScaleY)
                .def("getScaleZ",&Transform3D::getScaleZ)
                .def("setScaleX",&Transform3D::setScaleX)
                .def("setScaleY",&Transform3D::setScaleY)
                .def("setScaleZ",&Transform3D::setScaleZ)
                .def("scaleByX",&Transform3D::scaleByX)
                .def("scaleByY",&Transform3D::scaleByY)
                .def("scaleByZ",&Transform3D::scaleByZ)
                .def("setScale",static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setScale))
        ];
    }

    void
    LuaEngine::exposeTime
    ()
    {
        debugRegisteringClass("Time");
        module(mState) [
            class_<Time>("Time")
                .def("getCurrentTime",&Time::getCurrentTime)
                .def("getLastTime",&Time::getLastTime)
                .def("getTimeDelta",&Time::getTimeDelta)
                .def("scaleValue",&Time::scaleValue)
        ];
    }

    void
    LuaEngine::exposeAssimpModelInstance
    ()
    {
        debugRegisteringClass("AssimpModelInstance");
        module(mState)
                [
                class_<AssimpModelInstance>("AssimpModelInstance")
                // TODO
                ];
    }

    void
    LuaEngine::exposeEvent
    ()
    {
        debugRegisteringClass("Event");
        module(mState)
                [
                class_<Event>("Event")
                .def("getSender",&Event::getSender)
                .def("getType",&Event::getType)
                .def("getAttribute",&Event::getAttribute)
                .def("setAttribute",&Event::setAttribute)
                ];
    }

    void
    LuaEngine::exposeIAssetInstance
    ()
    {
        debugRegisteringClass("IAssetInstance");
        module(mState)
                [
                class_<IAssetInstance>("IAssetInstance")
                ];
    }

    void
    LuaEngine::exposeAudioComponent
    ()
    {
        debugRegisteringClass("AudioComponent");
        module(mState)
        [
            class_<AudioComponent>("AudioComponent")
                .def("play",&AudioComponent::playAudioAsset)
                .def("pause",&AudioComponent::pauseAudioAsset)
                .def("stop",&AudioComponent::stopAudioAsset)
        ];
    }


    void
    LuaEngine::exposeLuaScriptInstance
    ()
    {
        // TODO
    }

    void
    LuaEngine::exposeAudioInstance
    ()
    {
        debugRegisteringClass("AudioInstance");
        module(mState)
        [
            class_<AudioInstance>("AudioInstance")
                .def("getStatus",&AudioInstance::getStatus),

            class_<AudioStatus>("AudioStatus")
                .enum_("AudioStatus")
                [
                    value("PLAYING", AudioStatus::PLAYING),
                    value("PAUSED",  AudioStatus::PAUSED),
                    value("STOPPED", AudioStatus::STOPPED)
                ]
        ];
    }

    void
    LuaEngine::exposeInputEvent
    ()
    {
        debugRegisteringClass("InputEvent");
        module(mState)
        [
            class_<InputEvent>("InputEvent")

                .def("getSource",&InputEvent::getSource)
                .def("setSource",&InputEvent::setSource)

                .def("getMouseEventType",&InputEvent::getMouseEventType)
                .def("setMouseEventType",&InputEvent::setMouseEventType)

                .def("getKeyEventType",&InputEvent::getKeyEventType)
                .def("setKeyEventType",&InputEvent::setKeyEventType)

                .def("getGamepadEventType",&InputEvent::getGamepadEventType)
                .def("setGamepadEventType",&InputEvent::setGamepadEventType)

                .def("getX",&InputEvent::getX)
                .def("setX",&InputEvent::setX)

                .def("getY",&InputEvent::getY)
                .def("setY",&InputEvent::setY)

                .def("isPressed",&InputEvent::isPressed)
                .def("setPressed",&InputEvent::setPressed)

                .def("getKey",&InputEvent::getKey)
                .def("setKey",&InputEvent::setKey)

                .def("getButton",&InputEvent::getButton)
                .def("setButton",&InputEvent::setButton),

            class_<InputSource>("InputSource")
                .enum_("InputSource")
                [
                    value("INPUT_SOURCE_KEYBOARD",InputSource::INPUT_SOURCE_KEYBOARD),
                    value("INPUT_SOURCE_MOUSE",InputSource::INPUT_SOURCE_MOUSE),
                    value("INPUT_SOURCE_GAMEPAD",InputSource::INPUT_SOURCE_GAMEPAD),
                    value("INPUT_SOURCE_NONE",InputSource::INPUT_SOURCE_NONE)
                ],

            class_<KeyEventType>("KeyEventType")
                .enum_("KeyEventType")
                [
                    value("KEY_PRESSED",KeyEventType::KEY_PRESSED),
                    value("KEY_RELEASED",KeyEventType::KEY_RELEASED),
                    value("KEY_NONE",KeyEventType::KEY_NONE)
                ],

            class_<MouseEventType>("MouseEventType")
                .enum_("MouseEventType")
                [
                    value("MOUSE_BUTTON_PRESSED",MouseEventType::MOUSE_BUTTON_PRESSED),
                    value("MOUSE_BUTTON_RELEASED",MouseEventType::MOUSE_BUTTON_RELEASED),
                    value("MOUSE_SCROLL",MouseEventType::MOUSE_SCROLL),
                    value("MOUSE_MOTION",MouseEventType::MOUSE_MOTION),
                    value("MOUSE_NONE",MouseEventType::MOUSE_NONE)
                ],

            class_<GamepadEventType>("GamepadEventType")
                .enum_("GamepadEventType")
                [
                    value("GAMEPAD_AXIS",GamepadEventType::GAMEPAD_AXIS),
                    value("GAMEPAD_BUTTON_PRESSED",GamepadEventType::GAMEPAD_BUTTON_PRESSED),
                    value("GAMEPAD_BUTTON_RELEASED",GamepadEventType::GAMEPAD_BUTTON_RELEASED),
                    value("GAMEPAD_NONE",GamepadEventType::GAMEPAD_NONE)
                ]
        ];
    }

    void
    LuaEngine::exposeAPI
    ()
    {
        exposeAnimationComponent();
        exposeAnimationInstance();
        exposeAssimpModelInstance();
        exposeCamera();
        exposeProjectRuntime();
        exposeEvent();
        exposeFontInstance();
        exposeInputEvent();
        exposeGraphicsComponent();
        exposeIAssetInstance();
        exposeAudioInstance();
        exposeAudioComponent();
        exposeIWindowComponent();
        exposeLightInstance();
        exposeLuaScriptInstance();
        exposeMath();
        exposePhysicsComponent();
        exposePhysicsObjectInstance();
        exposeSceneObjectRuntime();
        exposeShaderInstance();
        exposeSpriteInstance();
        exposeTime();
        exposeTransform3D();
        exposeNanoVG();
    }

    void
    LuaEngine::removeFromScriptMap
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        map<SceneObjectRuntime*,LuaScriptInstance*>::iterator iter;
        for(iter = begin(mScriptMap); iter != end(mScriptMap); iter++)
        {
           if ((*iter).first == sceneObject)
           {
               string id = (*iter).first->getUuid();
               object reg = registry(mState);
               reg[id] = nil;

               string name = (*iter).first->getNameAndUuidString();
               log->info( "Removed script for {}" , name );

               mScriptMap.erase(iter++);
               break;
           }
        }
    }

    void
    LuaEngine::addToScriptMap
    (SceneObjectRuntime *sceneObject, LuaScriptInstance* script)
    {
        auto log = getLog();
        log->info(
            "Adding {} to script map for {}",
            script->getNameAndUuidString(),
            sceneObject->getNameAndUuidString()
        );

        if (createScript(sceneObject,script))
        {
            mScriptMap.insert(pair<SceneObjectRuntime*,LuaScriptInstance*>(sceneObject,script));
        }
    }

    void
    LuaEngine::exposeNanoVG
    ()
    {
        debugRegisteringClass("NanoVG");
        module(mState)[
            class_<NVGcolor>("NVGcolor"),
            class_<NVGpaint>("NVGpaint"),
            class_<NVGglyphPosition>("NVGglyphPosition"),
            class_<NVGtextRow>("NVGtextRow"),
            class_<NanoVGComponent>("NanoVG")
                .enum_("NVGsolidity")
                [
                    value("NVG_SOLID",NVG_SOLID),
                    value("NVG_HOLE",NVG_HOLE)
                ]
               .def("BeginFrame",&NanoVGComponent::BeginFrame)
               .def("CancelFrame",&NanoVGComponent::CancelFrame)
               .def("EndFrame",&NanoVGComponent::EndFrame)
               .def("GlobalCompositeOperation",&NanoVGComponent::GlobalCompositeOperation)
               .def("GlobalCompositeBlendFunc",&NanoVGComponent::GlobalCompositeBlendFunc)
               .def("GlobalCompositeBlendFuncSeparate",&NanoVGComponent::GlobalCompositeBlendFuncSeparate)
               .def("RGB",&NanoVGComponent::RGB)
               .def("RGBf",&NanoVGComponent::RGBf)
               .def("RGBA",&NanoVGComponent::RGBA)
               .def("RGBAf",&NanoVGComponent::RGBAf)
               .def("LerpRGBA",&NanoVGComponent::LerpRGBA)
               .def("TransRGBA",&NanoVGComponent::TransRGBA)
               .def("TransRGBAf",&NanoVGComponent::TransRGBAf)
               .def("HSL",&NanoVGComponent::HSL)
               .def("HSLA",&NanoVGComponent::HSLA)
               .def("Save",&NanoVGComponent::Save)
               .def("Restore",&NanoVGComponent::Restore)
               .def("Reset",&NanoVGComponent::Reset)
               .def("ShapeAntiAlias",&NanoVGComponent::ShapeAntiAlias)
               .def("StrokeColor",&NanoVGComponent::StrokeColor)
               .def("StrokePaint",&NanoVGComponent::StrokePaint)
               .def("FillColor",&NanoVGComponent::FillColor)
               .def("FillPaint",&NanoVGComponent::FillPaint)
               .def("MiterLimit",&NanoVGComponent::MiterLimit)
               .def("StrokeWidth",&NanoVGComponent::StrokeWidth)
               .def("LineCap",&NanoVGComponent::LineCap)
               .def("LineJoin",&NanoVGComponent::LineJoin)
               .def("GlobalAlpha",&NanoVGComponent::GlobalAlpha)
               .def("ResetTransform",&NanoVGComponent::ResetTransform)
               .def("Transform",&NanoVGComponent::Transform)
               .def("Translate",&NanoVGComponent::Translate)
               .def("Rotate",&NanoVGComponent::Rotate)
               .def("SkewX",&NanoVGComponent::SkewX)
               .def("SkewY",&NanoVGComponent::SkewY)
               .def("Scale;",&NanoVGComponent::Scale)
               .def("CurrentTransform",&NanoVGComponent::CurrentTransform)
               .def("TransformIdentity",&NanoVGComponent::TransformIdentity)
               .def("TransformTranslate",&NanoVGComponent::TransformTranslate)
               .def("TransformScale",&NanoVGComponent::TransformScale)
               .def("TransformRotate",&NanoVGComponent::TransformRotate)
               .def("TransformSkewX",&NanoVGComponent::TransformSkewX)
               .def("TransformSkewY",&NanoVGComponent::TransformSkewY)
               .def("TransformMultiply",&NanoVGComponent::TransformMultiply)
               .def("TransformPremultiply",&NanoVGComponent::TransformPremultiply)
               .def("TransformInverse",&NanoVGComponent::TransformInverse)
               .def("TransformPoint",&NanoVGComponent::TransformPoint)
               .def("DegToRad",&NanoVGComponent::DegToRad)
               .def("RadToDeg",&NanoVGComponent::RadToDeg)
               .def("CreateImage",&NanoVGComponent::CreateImage)
               .def("CreateImageMem",&NanoVGComponent::CreateImageMem)
               .def("CreateImageRGBA",&NanoVGComponent::CreateImageRGBA)
               .def("UpdateImage",&NanoVGComponent::UpdateImage)
               .def("ImageSize",&NanoVGComponent::ImageSize)
               .def("DeleteImage",&NanoVGComponent::DeleteImage)
               .def("LinearGradient",&NanoVGComponent::LinearGradient)
               .def("BoxGradient",&NanoVGComponent::BoxGradient)
               .def("RadialGradient",&NanoVGComponent::RadialGradient)
               .def("ImagePattern",&NanoVGComponent::ImagePattern)
               .def("Scissor",&NanoVGComponent::Scissor)
               .def("IntersectScissor",&NanoVGComponent::IntersectScissor)
               .def("ResetScissor",&NanoVGComponent::ResetScissor)
               .def("BeginPath",&NanoVGComponent::BeginPath)
               .def("MoveTo",&NanoVGComponent::MoveTo)
               .def("LineTo",&NanoVGComponent::LineTo)
               .def("BezierTo",&NanoVGComponent::BezierTo)
               .def("QuadTo",&NanoVGComponent::QuadTo)
               .def("ArcTo",&NanoVGComponent::ArcTo)
               .def("ClosePath",&NanoVGComponent::ClosePath)
               .def("PathWinding",&NanoVGComponent::PathWinding)
               .def("Arc",&NanoVGComponent::Arc)
               .def("Rect",&NanoVGComponent::Rect)
               .def("RoundedRect",&NanoVGComponent::RoundedRect)
               .def("RoundedRectVarying",&NanoVGComponent::RoundedRectVarying)
               .def("Ellipse",&NanoVGComponent::Ellipse)
               .def("Circle",&NanoVGComponent::Circle)
               .def("Fill",&NanoVGComponent::Fill)
               .def("Stroke",&NanoVGComponent::Stroke)
               .def("CreateFont",&NanoVGComponent::CreateFont)
               .def("CreateFontMem",&NanoVGComponent::CreateFontMem)
               .def("FindFont",&NanoVGComponent::FindFont)
               .def("AddFallbackFontId",&NanoVGComponent::AddFallbackFontId)
               .def("AddFallbackFont",&NanoVGComponent::AddFallbackFont)
               .def("FontSize",&NanoVGComponent::FontSize)
               .def("FontBlur",&NanoVGComponent::FontBlur)
               .def("TextLetterSpacing",&NanoVGComponent::TextLetterSpacing)
               .def("TextLineHeight",&NanoVGComponent::TextLineHeight)
               .def("TextAlign",&NanoVGComponent::TextAlign)
               .def("FontFaceId",&NanoVGComponent::FontFaceId)
               .def("FontFace",&NanoVGComponent::FontFace)
               .def("Text",&NanoVGComponent::Text)
               .def("TextBox",&NanoVGComponent::TextBox)
               .def("TextBounds",&NanoVGComponent::TextBounds)
               .def("TextBoxBounds",&NanoVGComponent::TextBoxBounds)
               .def("TextGlyphPositions",&NanoVGComponent::TextGlyphPositions)
               .def("TextMetrics",&NanoVGComponent::TextMetrics)
               .def("TextBreakLines",&NanoVGComponent::TextBreakLines)
        ];
    }

} // End of Dream

