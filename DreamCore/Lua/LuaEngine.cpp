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

int
errorHandler
(lua_State *L)
{
    // log the error message
    object msg(from_stack( L, -1 ));
    ostringstream str;
    str << "Lua - RuntimeError: " << msg;
    cerr << str.str() << endl;
    // log the callstack
    string traceback = call_function<string>( globals(L)["debug"]["traceback"] );
    traceback = string( "Lua - Traceback: " ) + traceback;
    cerr << traceback.c_str() << endl;
    return 1;
}

namespace Dream
{
    LuaEngine::LuaEngine
    (ProjectRuntime* projectHandle, LuaScriptCache* cache)
        : mScriptCacheHandle(cache),
          mProjectRuntimeHandle(projectHandle)
    {
        {
            cout << "LuaEngine: Constructing Object" << endl;
        }
    }

    LuaEngine::~LuaEngine
    ()
    {
        {
            cout << "LuaEngine: Destroying Object" << endl;
        }

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
        {
            cout << "LuaEngine: Initialising LuaEngine" << endl;
        }
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
            cerr << "LuaEngine: Error creating lua state, LuaEngine::mState == nullptr"
                 << endl;
        }
        return false;
    }

    bool
    LuaEngine::createScript
    (SceneObjectRuntime* sceneObject, LuaScriptInstance* luaScript)
    {
        if (luaScript == nullptr)
        {
            cerr << "LuaEngine: Load Failed, LuaScriptInstance is NULL" << endl;
            return false;
        }

        if (sceneObject == nullptr)
        {
            cerr << "LuaEngine: Load Failed, SceneObjectRuntime is NULL" << endl;
            return false;
        }

        if (luaScript->getLoadedFlag())
        {
            {
               cout << "LuaEngine: Script " << luaScript->getNameAndUuidString()
                    << " is all ready loaded" << endl;
            }
            return false;
        }

        {
            cout << "LuaEngine: loading script '" << luaScript->getName()
                 << "' for '"
                 << sceneObject->getName()
                 << "'" << endl;
            cout << "LuaEngine: Loading Lua script from " << luaScript->getAbsolutePath()
                 << endl << flush;
        }

        if (!loadScript(sceneObject))
        {
            return false;
        }

        {
            cout << "LuaEngine: Loaded " << sceneObject->getUuid() << " Successfully" << endl;
        }

        luaScript->setLoadedFlag(true);
        executeScriptInit(sceneObject);

        return true;
    }

    bool
    LuaEngine::loadScript
    (SceneObjectRuntime* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (!mState)
        {
            cerr << "LuaEngine: Cannot load script, mState == nullptr!" << endl;
            return false;
        }

        if (scriptInstance->getError())
        {
           cerr << "LuaEngine: Cannot load script " << id << " while in error state" << endl;
           return false;
        }

        {
            cout << "LuaEngine: loadScript called for " << id << endl;
        }

        try
        {
            {
                cout << "LuaEngine: Creating new table for " << id << endl;
            }

            object newScriptTable = newtable(mState);
            string path = scriptInstance->getAbsolutePath();
            string script = mScriptCacheHandle->getScript(path);

            {
                cout << "LuaEngine: calling scriptLoadFromString in lua for " << id << endl;
            }

            call_function<void>(mState, "scriptLoadFromString", newScriptTable, script.c_str());

            object reg = registry(mState);
            reg[id] = newScriptTable;
        }
        catch (error &e)
        {
            cerr << "LuaEngine: loadScript exception:" << endl
                 << "\t" << e.what() << endl;
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    void
    LuaEngine::stackDump
    ()
    {
        cerr << "LuaEngine: Stack Dump!" << endl;
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
                    cerr << lua_tostring(mState, i);
                    break;
                    // booleans
                case LUA_TBOOLEAN:
                    cerr << (lua_toboolean(mState, i) ? "true" : "false");
                    break;
                    // numbers
                case LUA_TNUMBER:
                    cerr << lua_tonumber(mState, i);
                    break;
                    // other values
                default:
                    cout << lua_typename(mState, t);
                    break;
            }
            // put a separator
            cerr << " ";
        }
        // end the listing
        cerr << endl;
    }

    bool
    LuaEngine::update
    ()
    {
        {
            cout << "LuaEngine: Update Called" << endl;
        }

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
    LuaEngine::executeScriptUpdate
    (SceneObjectRuntime* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            cerr << "LuaEngine: Cannot execute " << scriptInstance->getNameAndUuidString()
                                              << " in error state" << endl;
            return false;
        }

        {
            cout << "LuaEngine: Calling onUpdate for " << sceneObject->getNameAndUuidString() << endl;
        }

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
                cerr << "LuaEngine: Attempted to call onUpdate on invalid function."<< endl;
            }
        }
        catch (error &e)
        {
            string error = lua_tostring( e.state(), -1 );
            cerr << "LuaEngine: onUpdate exception:" << endl << e.what() << endl;
            cerr << error << endl;
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    bool
    LuaEngine::executeScriptInit
    (SceneObjectRuntime* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            cerr << "LuaEngine: Cannot execute " << scriptInstance->getNameAndUuidString()
                                              << " in error state" << endl;

            return false;
        }

        {
            cout << "LuaEngine: Calling onInit in " << scriptInstance->getName()
                 <<" for " << sceneObject->getName()
                << endl;
        }
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
                cerr << "LuaEngine: Attempted to call onInit on invalid function."<< endl;
            }

        }
        catch (error &e)
        {
            string error = lua_tostring( e.state(), -1 );
            cerr << "LuaEngine: onInit exception:" << endl << e.what() << endl;
            cerr << error << endl;
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    bool
    LuaEngine::executeScriptInputHandler
    (SceneObjectRuntime* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            cerr << "LuaEngine: Cannot execute " << scriptInstance->getNameAndUuidString()
                                              << " in error state" << endl;

            return false;
        }


        {
            cout << "LuaEngine: Calling onInput for "
                 << sceneObject->getNameAndUuidString()
                 << " (Has Focus)" << endl << flush;
        }

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
                cerr << "LuaEngine: Attempted to call onInput on invalid function."<< endl;
            }

            clearInputEvents();
        }
        catch (error &e)
        {
            string error = lua_tostring( e.state(), -1 );
            cerr << "LuaEngine: onInput exception:" << endl << e.what() << endl;
            cerr << error << endl;
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    bool
    LuaEngine::executeScriptEventHandler
    (SceneObjectRuntime* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            cerr << "LuaEngine: Cannot execute " << scriptInstance->getNameAndUuidString()
                                              << " in error state" << endl;

            return false;
        }


        {
            cout << "LuaEngine: Calling onEvent for "
                 << sceneObject->getNameAndUuidString()
                 << endl;
        }

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
                cerr << "LuaEngine: Attempted to call onInit on invalid function."<< endl;
            }
        }
        catch (error &e)
        {
            string error = lua_tostring( e.state(), -1 );
            cerr << "LuaEngine: onEvent exception:"
                 << endl
                 << e.what()
                 << endl;
            cerr << error << endl;
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
            cout << "LuaEngine: Registering Class " << className << endl;
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
                .def("degreesToRadians",&Math::degreesToRadians)
                .def("radiansToDegrees",&Math::radiansToDegrees)
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
    }

    void
    LuaEngine::removeFromScriptMap
    (SceneObjectRuntime* sceneObject)
    {
        map<SceneObjectRuntime*,LuaScriptInstance*>::iterator iter;
        for(iter = begin(mScriptMap); iter != end(mScriptMap); iter++)
        {
           if ((*iter).first == sceneObject)
           {
               string id = (*iter).first->getUuid();
               object reg = registry(mState);
               reg[id] = nil;

               {
                    cout << "LuaEngine: Removed script for " << (*iter).first->getNameAndUuidString() << endl;
               }

               mScriptMap.erase(iter++);
               break;
           }
        }
    }

    void
    LuaEngine::addToScriptMap
    (SceneObjectRuntime *sceneObject, LuaScriptInstance* script)
    {
        {
           cout << "LuaEngine: Adding " << script->getNameAndUuidString()
                << " to script map for " << sceneObject->getNameAndUuidString()
                <<endl;
        }

        if (createScript(sceneObject,script))
        {
            mScriptMap.insert(pair<SceneObjectRuntime*,LuaScriptInstance*>(sceneObject,script));
        }
    }

} // End of Dream

