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

#include "LuaScriptCache.h"
#include "../Project.h"
#include "../ProjectRuntime.h"
#include "../Math.h"
#include "../Components/Animation/AnimationComponent.h"
#include "../Components/Animation/AnimationInstance.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Audio/AudioInstance.h"
#include "../Components/Graphics/AssimpModelInstance.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/FontInstance.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/LightInstance.h"
#include "../Components/Graphics/ShaderInstance.h"
#include "../Components/Graphics/SpriteInstance.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Physics/PhysicsObjectInstance.h"
#include "../Components/Window/IWindowComponent.h"

using namespace std;

int
errorHandler
(lua_State *L)
{
    // log the error message
    luabind::object msg(luabind::from_stack( L, -1 ));
    std::ostringstream str;
    str << "Lua - RuntimeError: " << msg;
    cerr << str.str() << endl;
    // log the callstack
    std::string traceback = luabind::call_function<std::string>( luabind::globals(L)["debug"]["traceback"] );
    traceback = std::string( "Lua - Traceback: " ) + traceback;
    cerr << traceback.c_str() << endl;
    // return unmodified error object
    return 1;
}

namespace Dream
{
    LuaEngine::LuaEngine
    (Project* project)
    {
        mProject = project;
        mState = nullptr;
    }

    LuaEngine::~LuaEngine
    ()
    {
        if (DEBUG)
        {
            cout << "LuaEngine: Destroying Object" << endl;
        }

        if (mState != nullptr)
        {
            lua_close(mState);
        }
    }

    bool
    LuaEngine::init
    ()
    {
        if (DEBUG)
        {
            cout << "LuaEngine: Initialising LuaEngine" << endl;
        }
        mState = luaL_newstate();
        if (mState)
        {
            luabind::open(mState);
            luaopen_base(mState);
            luabind::set_pcall_callback(&errorHandler);
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
    LuaEngine::createAllScripts
    ()
    {
        if (VERBOSE)
        {
            cout << "LuaEngine: CreateAllScripts Called" << endl;
        }

        for (pair<SceneObject*,LuaScriptInstance*> entry : mScriptMap)
        {
            SceneObject *sceneObject = entry.first;
            LuaScriptInstance *luaScript = entry.second;

            if (luaScript == nullptr)
            {
                cerr << "LuaEngine: Load Failed, LuaScriptInstance is NULL" << endl;
                continue;
            }

            if (sceneObject == nullptr)
            {
                cerr << "LuaEngine: Load Failed, SceneObject is NULL" << endl;
                continue;
            }

            if (luaScript->getLoadedFlag())
            {
                continue;
            }

            if (sceneObject->getDeleteFlag())
            {

                if (DEBUG)
                {
                    cout << "LuaEngine: SceneObject marked for deletion, not loading script." << endl;
                }
                continue;
            }

            if (DEBUG)
            {
                cout << "LuaEngine: loading script '" << luaScript->getName() << "' for '" << sceneObject->getName() << "'" << endl;
                cout << "LuaEngine: Loading Lua script from " << luaScript->getAbsolutePath()
                     << endl << flush;
            }

            if (!loadScript(sceneObject))
            {
                return false;
            }

            if (DEBUG)
            {
                cout << "LuaEngine: Loaded " << sceneObject->getUuid() << " Successfully" << endl;
            }

            luaScript->setLoadedFlag(true);
            executeScriptInit(sceneObject);
        }

        return true;
    }

    bool
    LuaEngine::loadScript
    (SceneObject* sceneObject)
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

        if (DEBUG)
        {
            cout << "LuaEngine: loadScript called for " << id << endl;
        }

        try
        {
            if (DEBUG)
            {
                cout << "LuaEngine: Creating new table for " << id << endl;
            }

            luabind::object newtable = luabind::newtable(mState);
            string path = scriptInstance->getAbsolutePath();
            string script = LuaScriptCache::getScript(path);

            if (DEBUG)
            {
                cout << "LuaEngine: calling scriptLoadFromString in lua for " << id << endl;
            }

            luabind::call_function<void>(mState, "scriptLoadFromString", newtable, script.c_str());

            luabind::object reg = luabind::registry(mState);
            reg[id] = newtable;
        }
        catch (luabind::error &e)
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
        if (VERBOSE)
        {
            cout << "LuaEngine: Update Called" << endl;
        }

        for (pair<SceneObject*,LuaScriptInstance*> entry : mScriptMap)
        {
            SceneObject* key = entry.first;
            if (key->getDeleteFlag())
            {
                if (DEBUG)
                {
                    cout << "LuaEngine: Skipping update on " << key->getUuid() << endl;
                }
                continue;
            }

            if (!executeScriptUpdate(key))
            {
                return false;
            }

            if (key->hasFocus())
            {
                if (!executeScriptKeyHandler(key))
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
    (SceneObject* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            return false;
        }

        if (VERBOSE)
        {
            cout << "LuaEngine: Calling onUpdate for " << sceneObject->getNameAndUuidString() << endl;
        }

        try
        {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_UPDATE_FUNCTION];
            luabind::call_function<void>(funq,sceneObject);
        }
        catch (luabind::error &e)
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
    (SceneObject* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            return false;
        }

        if (VERBOSE)
        {
            cout << "LuaEngine: Calling onInit in " << scriptInstance->getName()
                 <<" for " << sceneObject->getName()
                << endl;
        }
        try
        {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_INIT_FUNCTION];
            luabind::call_function<void>(funq,sceneObject);
        }
        catch (luabind::error &e)
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
    LuaEngine::executeScriptKeyHandler
    (SceneObject* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            return false;
        }


        if (VERBOSE)
        {
            cout << "LuaEngine: Calling onInput for " << sceneObject->getNameAndUuidString() << endl << flush;
        }
        try
        {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_INPUT_FUNCTION];
        }
        catch (luabind::error &e)
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
    (SceneObject* sceneObject)
    {
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = sceneObject->getScriptInstance();

        if (scriptInstance->getError())
        {
            return false;
        }


        if (VERBOSE)
        {
            cout << "LuaEngine: Calling onEvent for " << sceneObject->getNameAndUuidString() << endl << flush;
        }

        try
        {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_EVENT_FUNCTION];
            vector<Event*>* events = sceneObject->getEventQueue();
            for (Event* event : *events)
            {
                luabind::call_function<void>(funq,sceneObject,event);
            }
            sceneObject->cleanupEvents();
        }
        catch (luabind::error &e)
        {
            string error = lua_tostring( e.state(), -1 );
            cerr << "LuaEngine: onEvent exception:" << endl << e.what() << endl;
            cerr << error << endl;
            scriptInstance->setError(true);
            return false;
        }
        return true;
    }

    // API Exposure Methods ======================================================

    void
    LuaEngine::exposeProjectRuntime
    ()
    {
        luabind::module(mState)
        [
            luabind::class_<ProjectRuntime>("Runtime")
                .def("getAudioComponent",&ProjectRuntime::getAudioComponent)
                .def("getGraphicsComponent",&ProjectRuntime::getGraphicsComponent)
                .def("getPhysicsComponent",&ProjectRuntime::getPhysicsComponent)
                .def("getWindowComponent",&ProjectRuntime::getWindowComponent)
                .def("getTime",&ProjectRuntime::getTime)
        ];
        luabind::globals(mState)["Runtime"] = mProject->getRuntime();
    }

    void
    LuaEngine::exposeProject
    ()
    {
        luabind::module(mState)
        [
            luabind::class_<Project>("Project")
                .def("getActiveScene",&Project::getActiveScene)
        ];
        luabind::globals(mState)["Project"] = mProject;
    }

    void
    LuaEngine::exposeCamera
    ()
    {
        luabind::module(mState)
                [
                luabind::class_<Camera>("Camera")
                .def("processKeyboard",&Camera::processKeyboard)
                .def("processMouseMovement",&Camera::processMouseMovement)
                .enum_("CameraMovement")
                [
                luabind::value("FORWARD",  CAMERA_MOVEMENT_FORWARD),
                luabind::value("BACKWARD", CAMERA_MOVEMENT_BACKWARD),
                luabind::value("LEFT",     CAMERA_MOVEMENT_LEFT),
                luabind::value("RIGHT",    CAMERA_MOVEMENT_RIGHT)
                ]
                ];
    }

    void LuaEngine::debugRegisteringClass(string className)
    {
        if (VERBOSE)
            cout << "LuaEngine: Registering Class " << className << endl;
        return;
    }

    void
    LuaEngine::exposeAnimationComponent
    ()
    {
        debugRegisteringClass("AnimationComponent");
        luabind::module(mState)
                [
                luabind::class_<AnimationComponent>("AnimationComponent")
                // TODO
                ];
    }

    void
    LuaEngine::exposeAnimationInstance
    ()
    {
        debugRegisteringClass("AnimationInstance");
        luabind::module(mState)
                [
                luabind::class_<AnimationInstance>("AnimationInstance")
                // TODO
                ];
    }

    void
    LuaEngine::exposeFontInstance
    ()
    {
        debugRegisteringClass("FontInstance");
        luabind::module(mState)
                [
                luabind::class_<FontInstance>("FontInstance")
                .def("setText",&FontInstance::setText)
                .def("getText",&FontInstance::getText)
                ];
    }

    void
    LuaEngine::exposeGraphicsComponent
    ()
    {
        debugRegisteringClass("GraphicsComponent");
        luabind::module(mState)
                [
                luabind::class_<GraphicsComponent>("GraphicsComponent")
                    .def("getCamera",&GraphicsComponent::getCamera)
                ];
    }

    void
    LuaEngine::exposeLightInstance
    ()
    {
        debugRegisteringClass("LightInstance");
        luabind::module(mState)
                [
                luabind::class_<LightInstance>("LightInstance")
                // TODO
                ];
    }

    void
    LuaEngine::exposeShaderInstance
    ()
    {
        debugRegisteringClass("ShaderInstance");
        luabind::module(mState)
                [
                luabind::class_<ShaderInstance>("ShaderInstance")
                .def("getUuid", &ShaderInstance::getUuid)
                .def("setUniform1f", &ShaderInstance::setUniform1f)
                ];
    }

    void
    LuaEngine::exposeSpriteInstance
    ()
    {
        debugRegisteringClass("SpriteInstance");
        luabind::module(mState)
                [
                luabind::class_<SpriteInstance>("SpriteInstance")
                // TODO
                ];
    }

    void
    LuaEngine::exposePhysicsComponent
    ()
    {
        debugRegisteringClass("PhysicsComponent");
        luabind::module(mState)
                [
                luabind::class_<PhysicsComponent>("PhysicsComponent")
                .def("setDebug",&PhysicsComponent::setDebug)
                ];
    }

    void
    LuaEngine::exposePhysicsObjectInstance
    ()
    {
        debugRegisteringClass("PhysicsObjectInstance");
        luabind::module(mState)
                [
                luabind::class_<PhysicsObjectInstance>("PhysicsObjectInstance")
                .def("getUuid", &PhysicsObjectInstance::getUuid)
                .def("setLinearVelocity", &PhysicsObjectInstance::setLinearVelocity)
                ];
    }

    void
    LuaEngine::exposeIWindowComponent
    ()
    {
        debugRegisteringClass("IWindiwComponent");
        luabind::module(mState)
                [
                luabind::class_<IWindowComponent>("IWindowComponent")
                .def("getWidth",&IWindowComponent::getWidth)
                .def("getHeight",&IWindowComponent::getHeight)
                .def("setShouldClose",&IWindowComponent::setShouldClose)
                ];
    }

    void
    LuaEngine::exposeGameController
    ()
    {
        debugRegisteringClass("GameController");
        luabind::module(mState)
                [
                luabind::class_<GameController>("GameController")
                .def("getButtonValue",&GameController::getButtonValue)
                .def("getAxisValue",&GameController::getAxisValue)
                .enum_("ControllerButton")
                [
                // Face Buttons
                luabind::value("A_BTN",ControllerButton::A),
                luabind::value("B_BTN",ControllerButton::B),
                luabind::value("X_BTN",ControllerButton::X),
                luabind::value("Y_BTN",ControllerButton::Y),
                // Shoulders
                luabind::value("LEFT_SHOULDER",ControllerButton::LEFT_SHOULDER),
                luabind::value("RIGHT_SHOULDER",ControllerButton::RIGHT_SHOULDER),
                // Analog
                luabind::value("LEFT_ANALOG_BUTTON",ControllerButton::LEFT_ANALOG),
                luabind::value("RIGHT_ANALOG_BUTTON",ControllerButton::RIGHT_ANALOG),
                // D-Pad
                luabind::value("DPAD_UP",ControllerButton::DPAD_UP),
                luabind::value("DPAD_DOWN",ControllerButton::DPAD_DOWN),
                luabind::value("DPAD_LEFT",ControllerButton::DPAD_LEFT),
                luabind::value("DPAD_RIGHT",ControllerButton::DPAD_RIGHT)
                ]
                .enum_("ControllerAxis")
                [
                // Left Analog
                luabind::value("LEFT_ANALOG_X",ControllerAxis::LEFT_ANALOG_X),
                luabind::value("LEFT_ANALOG_Y",ControllerAxis::LEFT_ANALOG_Y),
                // Right Analog
                luabind::value("RIGHT_ANALOG_X",ControllerAxis::RIGHT_ANALOG_X),
                luabind::value("RIGHT_ANALOG_Y",ControllerAxis::RIGHT_ANALOG_Y),
                // Triggers
                luabind::value("LEFT_TRIGGER",ControllerAxis::LEFT_TRIGGER),
                luabind::value("RIGHT_TRIGGER",ControllerAxis::RIGHT_TRIGGER)
                ]
                ];
    }

    void
    LuaEngine::exposeMath
    ()
    {
        debugRegisteringClass("Math");
        luabind::module(mState)
                [
                luabind::class_<Math>("Math")
                .def("degreesToRadians",&Math::degreesToRadians)
                .def("radiansToDegrees",&Math::radiansToDegrees)
                ];
    }

    void
    LuaEngine::exposeScene
    ()
    {
        debugRegisteringClass("Scene");
        luabind::module(mState) [
                luabind::class_<Scene>("Scene")
                .def("getCameraMovementSpeed",&Scene::getCameraMovementSpeed)
                .def("setCameraMovementSpeed",&Scene::setCameraMovementSpeed)
                .def("getSceneObjectByUuid",&Scene::getSceneObjectByUuid)
                .def("getSceneObjectByName",&Scene::getSceneObjectByName)
                ];
    }

    void
    LuaEngine::exposeSceneObject
    ()
    {
        debugRegisteringClass("SceneObject");
        luabind::module(mState)
                [
                luabind::class_<SceneObject>("SceneObject")
                .def(luabind::constructor<>())
                .def("getUuid",&SceneObject::getUuid)
                .def("setUuid",&SceneObject::setUuid)
                .def("getName",&SceneObject::getName)
                .def("setName",&SceneObject::setName)
                .def("showStatus",&SceneObject::showStatus)
                .def("getChildByUuid",&SceneObject::getChildByUuid)
                .def("getTransform",&SceneObject::getTransform)
                .def("setTransform",&SceneObject::setTransform)
                .def("getParent",&SceneObject::getParent)
                .def("setParent",&SceneObject::setParent)
                .def("addChild",&SceneObject::addChild,luabind::adopt(boost::placeholders::_2))
                .def("addAssetDefUuidToLoad",&SceneObject::addAssetDefUuidToLoad)
                .def("getAnimationInstance",&SceneObject::getAnimationInstance)
                .def("setAnimationInstance",&SceneObject::setAnimationInstance)
                .def("getAudioInstance",&SceneObject::getAudioInstance)
                .def("setAudioInstance",&SceneObject::setAudioInstance)
                .def("getSpriteInstance",&SceneObject::getSpriteInstance)
                .def("setSpriteInstance",&SceneObject::setSpriteInstance)
                .def("getModelInstance",&SceneObject::getModelInstance)
                .def("setModelInstance",&SceneObject::setModelInstance)
                .def("getShaderInstance",&SceneObject::getShaderInstance)
                .def("setShaderInstance",&SceneObject::setShaderInstance)
                .def("getLightInstance",&SceneObject::getLightInstance)
                .def("setLightInstance",&SceneObject::setLightInstance)
                .def("getFontInstance",&SceneObject::getFontInstance)
                .def("setFontInstance",&SceneObject::setFontInstance)
                .def("getPhysicsObjectInstance",&SceneObject::getPhysicsObjectInstance)
                .def("setPhysicsObjectInstance",&SceneObject::setPhysicsObjectInstance)
                .def("getDeleteFlag",&SceneObject::getDeleteFlag)
                .def("setDeleteFlag",&SceneObject::setDeleteFlag)
                ];
    }

    void
    LuaEngine::exposeTransform3D
    ()
    {
        debugRegisteringClass("Transform3D");
        luabind::module(mState) [
                luabind::class_<Transform3D>("Transform3D")
                .def(luabind::constructor<>())
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
                .def("setTranslation",
                     static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setTranslation)
                     )
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
                .def("setRotation",
                     static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setRotation)
                     )
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
                .def("setScale",
                     static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setScale)
                     )
                ];
    }

    void
    LuaEngine::exposeTime
    ()
    {
        debugRegisteringClass("Time");
        luabind::module(mState) [
                luabind::class_<Time>("Time")
                .def("getCurrentTime",&Time::getCurrentTime)
                .def("getLastTime",&Time::getLastTime)
                .def("getTimeDelta",&Time::getTimeDelta)
                ];
    }

    void
    LuaEngine::exposeAssimpModelInstance
    ()
    {
        debugRegisteringClass("AssimpModelInstance");
        luabind::module(mState)
                [
                luabind::class_<AssimpModelInstance>("AssimpModelInstance")
                // TODO
                ];
    }

    void
    LuaEngine::exposeEvent
    ()
    {
        debugRegisteringClass("Event");
        luabind::module(mState)
                [
                luabind::class_<Event>("Event")
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
        luabind::module(mState)
                [
                luabind::class_<IAssetInstance>("IAssetInstance")
                ];
    }

    void
    LuaEngine::exposeAudioComponent
    ()
    {
        luabind::module(mState)
        [
                luabind::class_<AudioComponent>("AudioComponent")
                    .def("playAudioAsset",&AudioComponent::playAudioAsset)
                    .def("pauseAudioAsset",&AudioComponent::pauseAudioAsset)
                    .def("stopAudioAsset",&AudioComponent::stopAudioAsset)
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
        luabind::module(mState)
        [
            luabind::class_<AudioInstance>("AudioInstance")
                .def("play",&AudioInstance::play)
                .def("pause",&AudioInstance::pause)
                .def("stop",&AudioInstance::stop)
                .def("getStatus",&AudioInstance::getStatus),

            luabind::class_<AudioStatus>("AudioStatus")
                .enum_("AudioStatus")
                [
                    luabind::value("PLAYING", AudioStatus::PLAYING),
                    luabind::value("PAUSED",  AudioStatus::PAUSED),
                    luabind::value("STOPPED", AudioStatus::STOPPED)
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
        exposeProject();
        exposeProjectRuntime();
        exposeEvent();
        exposeFontInstance();
        exposeGameController();
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
        exposeScene();
        exposeSceneObject();
        exposeShaderInstance();
        exposeSpriteInstance();
        exposeTime();
        exposeTransform3D();
    }

    void
    LuaEngine::cleanUp
    (Scene* scene)
    {
        if (DEBUG)
        {
            cout << "LuaEngine: Cleaning up" << endl;
        }

        luabind::object reg = luabind::registry(mState);

        for(pair<SceneObject*,LuaScriptInstance*> scriptPair : mScriptMap)
        {
            string id = scriptPair.first->getUuid();
            string soName = scriptPair.first->getName();

            reg[id] = luabind::nil;

            if (DEBUG)
            {
                cout << "LuaEngine: Removed script " << id << " for " << soName << " from registry" << endl;
            }
        }
        mScriptMap.clear();
    }
    void
    LuaEngine::removeFromScriptMap
    (SceneObject* sceneObject)
    {
        map<SceneObject*,LuaScriptInstance*>::iterator iter;
        for(iter = begin(mScriptMap); iter != end(mScriptMap); iter++)
        {
           if ((*iter).first == sceneObject)
           {
               mScriptMap.erase(iter++);
               break;
           }
        }
    }

    void
    LuaEngine::addToScriptMap
    (SceneObject *sceneObject, LuaScriptInstance* script)
    {
        mScriptMap.insert(pair<SceneObject*,LuaScriptInstance*>(sceneObject,script));
    }

} // End of Dream

