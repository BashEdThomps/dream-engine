/*
 * LuaComponent
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



#include "LuaComponent.h"
#include "LuaScriptInstance.h"

#include <sstream>

#include "../../Event.h"
#include "../../Transform3D.h"
#include "../../Time.h"
#include "../../Path/PathComponent.h"
#include "../../Path/PathInstance.h"
#include "../../Audio/AudioComponent.h"
#include "../../Audio/AudioInstance.h"
#include "../../Graphics/Model/AssimpModelInstance.h"
#include "../../Graphics/Camera.h"
#include "../../Graphics/Font/FontInstance.h"
#include "../../Graphics/GraphicsComponent.h"
#include "../../Graphics/NanoVGComponent.h"
#include "../../Graphics/Light/LightInstance.h"
#include "../../Graphics/Shader/ShaderInstance.h"
#include "../../Graphics/Sprite/SpriteInstance.h"
#include "../../Input/InputComponent.h"
#include "../../Physics/PhysicsComponent.h"
#include "../../Physics/PhysicsObjectInstance.h"
#include "../../Window/IWindowComponent.h"
#include "../../../Project/ProjectRuntime.h"
#include "../../../Scene/SceneRuntime.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../../../NanoVG/src/nanovg.h"

#define SOL_CHECK_ARGUMENTS 1
//#define SOL_SAFE_FUNCTION 1
#include "../../../include/sol.hpp"

using std::ostringstream;
using std::exception;
using std::cout;
using std::cerr;
using std::string;

namespace Dream
{
    LuaComponent::LuaComponent
    (ProjectRuntime* project, ScriptCache* cache)
        : IScriptComponent(project, cache)

    {
        setLogClassName("LuaComponent");
        auto log = getLog();
        log->trace( "Constructing Object" );
    }

    LuaComponent::~LuaComponent
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );
        mScriptMap.clear();
        lua_close(mState);
    }

    bool
    LuaComponent::init
    ()
    {
        auto log = getLog();
        log->info( "Initialising LuaComponent" );
        mState = luaL_newstate();
        sol::state_view sView(mState);
        sView.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::math,
            sol::lib::string
        );
        log->info( "Got a sol state" );
        exposeAPI();
        return true;
    }

    bool
    LuaComponent::createScript
    (SceneObjectRuntime* sceneObject, ScriptInstance* script)
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

        return true;
    }

    /*
     * Instanciate the script and put into global registry
     */
    bool
    LuaComponent::loadScript
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();

        log->info( "loadScript called for {}", id );

        LuaScriptInstance* scriptInstance = dynamic_cast<LuaScriptInstance*>(sceneObject->getScriptInstance());

        if (scriptInstance->getError())
        {
            log->error( "Cannot load script {} while in error state",id );
            return false;
        }

        string path = scriptInstance->getAbsolutePath();
        string script = mScriptCache->getScript(path);

        log->info( "calling scriptLoadFromString in lua for {}" , id );

        sol::state_view solStateView(mState);
        sol::environment environment(mState, sol::create, solStateView.globals());

        solStateView[sceneObject->getUuid()] = environment;

        auto exec_result = solStateView.script(
            script,
            environment,
            [](lua_State*, sol::protected_function_result pfr)
            {
                // pfr will contain things that went wrong, for either loading or executing the script
                // Can throw your own custom error
                // You can also just return it, and let the call-site handle the error if necessary.
                return pfr;
            }
        );
        // it did not work
        if(!exec_result.valid())
        {
           // An error has occured
           sol::error err = exec_result;
           std::string what = err.what();
           log->critical("{}:\nCould not execute lua script:\n{}",
                      sceneObject->getNameAndUuidString(),what);
           scriptInstance->setError(true);
           return false;
        }

        log->info("Loaded Script Successfully");

        return true;
    }

    void
    LuaComponent::updateComponent
    ()
    {
        beginUpdate();
        auto log = getLog();
        log->info( "Update Called" );

        for (auto entry : mScriptMap)
        {
            auto sceneObj = entry.first;
            auto scriptObj =  entry.second;

            if (!scriptObj->getInitialised())
            {
                executeScriptInit(sceneObj);
            }
            else
            {
                executeScriptUpdate(sceneObj);

                if (sceneObj->hasFocus())
                {
                    executeScriptInput(sceneObj);
                }

                if (sceneObj->hasEvents())
                {
                    executeScriptEvent(sceneObj);
                }
            }
        }
        endUpdate();
    }

    bool
    LuaComponent::updateNanoVG
    ()
    {
        auto log = getLog();
        log->info("UpdateNanoVG Called");

        for (auto entry : mScriptMap)
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
    LuaComponent::executeScriptUpdate
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = dynamic_cast<LuaScriptInstance*>(sceneObject->getScriptInstance());

        if (scriptInstance->getError())
        {
            log->error("Cannot execute {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info("Calling onUpdate for {}",sceneObject->getNameAndUuidString() );

        sol::state_view solStateView(mState);
        sol::function onUpdateFunction = solStateView[sceneObject->getUuid()][Constants::LUA_UPDATE_FUNCTION];

        auto result = onUpdateFunction(sceneObject);

        if (!result.valid())
        {
            // An error has occured
           sol::error err = result;
           std::string what = err.what();
           log->critical
            (
               "{}:\nCould not execute onUpdate in lua script:\n{}",
                sceneObject->getNameAndUuidString(),
                what
            );
           scriptInstance->setError(true);
           return false;
        }

        return true;
    }

    bool
    LuaComponent::executeScriptNanoVG
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = dynamic_cast<LuaScriptInstance*>(sceneObject->getScriptInstance());

        if (scriptInstance->getError())
        {
            log->error("Cannot execute NanoVG {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info("Calling onNanoVG for {}" , sceneObject->getNameAndUuidString() );
        return true;
    }

    bool
    LuaComponent::executeScriptInit
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = dynamic_cast<LuaScriptInstance*>(sceneObject->getScriptInstance());

        if (scriptInstance->getError() )
        {
            log->error("Cannot execute {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        if (scriptInstance->getInitialised())
        {
            log->error("Script has all ready been initialised {}",scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info("Calling onInit in {} for {}",  scriptInstance->getName(),  sceneObject->getName());

        sol::state_view solStateView(mState);
        sol::function onInitFunction = solStateView[sceneObject->getUuid()][Constants::LUA_INIT_FUNCTION];

        auto initResult = onInitFunction(sceneObject);

        if (!initResult.valid())
        {
            // An error has occured
           sol::error err = initResult;
           std::string what = err.what();
           log->critical
           (
                "{}\nCould not execute onInit in lua script:\n{}",
                sceneObject->getNameAndUuidString(),
                what
            );
           scriptInstance->setError(true);
           return false;
        }

        scriptInstance->setInitialised(true);

        return true;
    }

    bool
    LuaComponent::executeScriptInput
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = dynamic_cast<LuaScriptInstance*>(sceneObject->getScriptInstance());

        if (scriptInstance->getError())
        {
            log->error("Cannot execute {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info("Calling onInput for {} (Has Focus) {}", sceneObject->getNameAndUuidString());

        sol::state_view solStateView(mState);
        sol::function onInputFunction = solStateView[sceneObject->getUuid()][Constants::LUA_INPUT_FUNCTION];

        auto result = onInputFunction(sceneObject,mInputMap);

        if (!result.valid())
        {
            // An error has occured
           sol::error err = result;
           std::string what = err.what();
           log->critical
            (
                "{}:\n"
                "Could not execute onInput in lua script:\n"
                "{}",
                sceneObject->getNameAndUuidString(),
                what
            );
           scriptInstance->setError(true);
           return false;
        }

        return true;
    }

    bool
    LuaComponent::executeScriptEvent
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        LuaScriptInstance* scriptInstance = dynamic_cast<LuaScriptInstance*>(sceneObject->getScriptInstance());

        if (scriptInstance->getError())
        {
            log->error( "Cannot execute {} in error state ",  scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info( "Calling onEvent for {}", sceneObject->getNameAndUuidString());

        sol::state_view solStateView(mState);
        sol::function onEventFunction = solStateView[sceneObject->getUuid()][Constants::LUA_EVENT_FUNCTION];

        for (Event e : sceneObject->getEventQueue())
        {
            auto result = onEventFunction(sceneObject,e);

            if (!result.valid())
            {
                // An error has occured
               sol::error err = result;
               std::string what = err.what();
               log->error
                (
                   "{}:\n"
                   "Could not execute onEvent in lua script:\n"
                   "{}",
                    sceneObject->getNameAndUuidString(),
                    what
                );
               scriptInstance->setError(true);
               return false;
            }
        }

        sceneObject->clearEventQueue();

        return true;
    }

    // API Exposure Methods ======================================================



    void
    LuaComponent::exposeProjectRuntime
    ()
    {
        debugRegisteringClass("ProjectRuntime");
        sol::state_view stateView(mState);
        stateView.new_usertype<ProjectRuntime>("ProjectRuntime",
            "getAudioComponent",&ProjectRuntime::getAudioComponent,
            "getGraphicsComponent",&ProjectRuntime::getGraphicsComponent,
            "getNanoVGComponent",&ProjectRuntime::getNanoVGComponent,
            "getPhysicsComponent",&ProjectRuntime::getPhysicsComponent,
            "getWindowComponent",&ProjectRuntime::getWindowComponent,
            "getTime",&ProjectRuntime::getTime,
            "getCamera",&ProjectRuntime::getCamera
        );
        stateView["Runtime"] = mProjectRuntime;
    }

    void
    LuaComponent::exposeCamera
    ()
    {
        debugRegisteringClass("Camera");
        sol::state_view stateView(mState);
        stateView.new_usertype<Camera>("Camera",
            "processKeyboard",&Camera::processKeyboard,
            "processMouseMovement",&Camera::processMouseMovement,
            "pan",&Camera::pan,
            "flyForward",&Camera::flyForward,
            "flyBackward",&Camera::flyBackward,
            "flyLeft",&Camera::flyLeft,
            "flyRight",&Camera::flyRight,
            "flyUp",&Camera::flyUp,
            "flyDown",&Camera::flyDown,
            "flyX",&Camera::flyX,
            "flyY",&Camera::flyY,
            "flyZ",&Camera::flyZ,
            "setFreeMode",&Camera::setFreeMode,
            "setLookAt",static_cast<void(Camera::*)(float,float,float)>(&Camera::setLookAt),
            "setLookAt",static_cast<void(Camera::*)(vec3)>( &Camera::setLookAt),
            "setTranslation",static_cast<void(Camera::*)(float,float,float)>(&Camera::setTranslation),
            "setTranslation",static_cast<void(Camera::*)(vec3)>(&Camera::setTranslation),
            "getTranslation",&Camera::getTranslation,
            "orbit",&Camera::orbit
        );

        stateView["Camera"] = mProjectRuntime->getCamera();

        /*
            .enum_("CameraMovement")
            [
                value("FORWARD",  Constants::CAMERA_MOVEMENT_FORWARD),
                value("BACKWARD", Constants::CAMERA_MOVEMENT_BACKWARD),
                value("LEFT",     Constants::CAMERA_MOVEMENT_LEFT),
                value("RIGHT",    Constants::CAMERA_MOVEMENT_RIGHT)
            ]
        */
    }

    void
    LuaComponent::exposePathComponent
    ()
    {
        debugRegisteringClass("PathComponent");
        sol::state_view stateView(mState);
        stateView.new_usertype<PathComponent>("PathComponent");
    }

    void
    LuaComponent::exposePathInstance
    ()
    {
        debugRegisteringClass("PathInstance");
        sol::state_view stateView(mState);
        stateView.new_usertype<PathInstance>("PathInstance",
            "generate",&PathInstance::generate,
            "getSplinePoints",&PathInstance::getSplinePoints,
            "getSplinePoint",&PathInstance::getSplinePoint,
            "getUStep",&PathInstance::getUStep,
            "setUStep",&PathInstance::setUStep,
            "stepPath",&PathInstance::stepPath
        );
    }

    void
    LuaComponent::exposeFontInstance
    ()
    {
        debugRegisteringClass("FontInstance");
        sol::state_view stateView(mState);
        stateView.new_usertype<FontInstance>("FontInstance",
            "setText",&FontInstance::setText,
            "getText",&FontInstance::getText
        );
    }

    void
    LuaComponent::exposeGraphicsComponent
    ()
    {
        debugRegisteringClass("GraphicsComponent");
        sol::state_view stateView(mState);
        stateView.new_usertype<GraphicsComponent>("GraphicsComponent");
    }

    void
    LuaComponent::exposeLightInstance
    ()
    {
        debugRegisteringClass("LightInstance");
        sol::state_view stateView(mState);
        stateView.new_usertype<LightInstance>("LightInstance");
    }

    void
    LuaComponent::exposeShaderInstance
    ()
    {
        debugRegisteringClass("ShaderInstance");
        sol::state_view stateView(mState);
        stateView.new_usertype<ShaderInstance>("ShaderInstance",
            "getUuid", &ShaderInstance::getUuid
        );

        debugRegisteringClass("ShaderUniform");

        stateView.new_usertype<ShaderUniform>("ShaderUniform");
        /*
                .enum_("UniformType")
                [
                    value("INT1",UniformType::INT1),
                    value("INT2",UniformType::INT2),
                    value("INT3",UniformType::INT3),
                    value("INT4",UniformType::INT4),
                    value("UINT1",UniformType::UINT1),
                    value("UINT2",UniformType::UINT2),
                    value("UINT3",UniformType::UINT3),
                    value("UINT4",UniformType::UINT4),
                    value("FLOAT1",UniformType::FLOAT1),
                    value("FLOAT2",UniformType::FLOAT2),
                    value("FLOAT3",UniformType::FLOAT3),
                    value("FLOAT4",UniformType::FLOAT4)
                ]
                */
    }

    void
    LuaComponent::exposeSpriteInstance
    ()
    {
        debugRegisteringClass("SpriteInstance");
        sol::state_view stateView(mState);
        stateView.new_usertype<SpriteInstance>("SpriteInstance");
    }

    void
    LuaComponent::exposePhysicsComponent
    ()
    {
        debugRegisteringClass("PhysicsComponent");
        sol::state_view stateView(mState);
        stateView.new_usertype<PhysicsComponent>("PhysicsComponent",
            "setDebug",&PhysicsComponent::setDebug
        );
    }

    void
    LuaComponent::exposePhysicsObjectInstance
    ()
    {
        debugRegisteringClass("PhysicsObjectInstance");
        sol::state_view stateView(mState);
        stateView.new_usertype<PhysicsObjectInstance>("PhysicsObjectInstance",
            "getUuid", &PhysicsObjectInstance::getUuid,

            "getLinearVelocity", &PhysicsObjectInstance::getLinearVelocity,
            "setLinearVelocity", &PhysicsObjectInstance::setLinearVelocity,

            "setLinearFactor", &PhysicsObjectInstance::setLinearFactor,
            "setAngularFactor", &PhysicsObjectInstance::setAngularFactor,

            "getRestitution", &PhysicsObjectInstance::getRestitution,
            "setRestitution", &PhysicsObjectInstance::setRestitution,

            "getFriction", &PhysicsObjectInstance::getFriction,
            "setFriction", &PhysicsObjectInstance::setFriction
        );
    }

    void
    LuaComponent::exposeSceneObjectRuntime
    ()
    {
        debugRegisteringClass("SceneObjectRuntime");
        sol::state_view stateView(mState);
        stateView.new_usertype<SceneObjectRuntime>("SceneObjectRuntime",
            "getName",&SceneObjectRuntime::getName,
            "getNameAndUuidString",&SceneObjectRuntime::getNameAndUuidString,
            "getChildByUuid",&SceneObjectRuntime::getChildRuntimeByUuid,
            "getParent",&SceneObjectRuntime::getParentRuntime,
            "setParent",&SceneObjectRuntime::setParentRuntime,
            "addAssetDefinitionUuidToLoad",&SceneObjectRuntime::addAssetDefinitionUuidToLoad,

            "getTransform",&SceneObjectRuntime::getTransform,
            "setTransform",&SceneObjectRuntime::setTransform,
            "walk",&SceneObjectRuntime::walk,

            "getPath",&SceneObjectRuntime::getPathInstance,
            "getAudio",&SceneObjectRuntime::getAudioInstance,
            "getSprite",&SceneObjectRuntime::getSpriteInstance,
            "getModel",&SceneObjectRuntime::getModelInstance,
            "getShader",&SceneObjectRuntime::getShaderInstance,
            "getLight",&SceneObjectRuntime::getLightInstance,
            "getFont",&SceneObjectRuntime::getFontInstance,
            "getPhysicsObject",&SceneObjectRuntime::getPhysicsObjectInstance,

            "hasPath",&SceneObjectRuntime::hasPathInstance,
            "hasAudio",&SceneObjectRuntime::hasAudioInstance,
            "hasSprite",&SceneObjectRuntime::hasSpriteInstance,
            "hasModel",&SceneObjectRuntime::hasModelInstance,
            "hasShader",&SceneObjectRuntime::hasShaderInstance,
            "hasLight",&SceneObjectRuntime::hasLightInstance,
            "hasFont",&SceneObjectRuntime::hasFontInstance,
            "hasPhysicsObject",&SceneObjectRuntime::hasPhysicsObjectInstance,

            "getDeleted",&SceneObjectRuntime::getDeleted,
            "setDeleted",&SceneObjectRuntime::setDeleted,

            "getHidden",&SceneObjectRuntime::getHidden,
            "setHidden",&SceneObjectRuntime::setHidden,

            "removeAudio", &SceneObjectRuntime::removeAudioInstance,
            "removePath", &SceneObjectRuntime::removePathInstance,
            "removeModel", &SceneObjectRuntime::removeModelInstance,
            "removeShader", &SceneObjectRuntime::removeShaderInstance,
            "removeLight", &SceneObjectRuntime::removeLightInstance,
            "removeSprite", &SceneObjectRuntime::removeSpriteInstance,
            "removeScript", &SceneObjectRuntime::removeScriptInstance,
            "removePhysicsObject", &SceneObjectRuntime::removePhysicsObjectInstance,
            "removeFont", &SceneObjectRuntime::removeFontInstance,

            "addEvent",&SceneObjectRuntime::addEvent

        );
    }

    void
    LuaComponent::exposeTransform3D
    ()
    {
        debugRegisteringClass("Transform3D");
        sol::state_view stateView(mState);
        stateView.new_usertype<Transform3D>("Transform3D",
            // Translation ===========================================================
            "getTransformType",&Transform3D::getTransformType,
            "getTranslationX",&Transform3D::getTranslationX,
            "getTranslationY",&Transform3D::getTranslationY,
            "getTranslationZ",&Transform3D::getTranslationZ,
            "setTranslationX",&Transform3D::setTranslationX,
            "setTranslationY",&Transform3D::setTranslationY,
            "setTranslationZ",&Transform3D::setTranslationZ,
            "translateByX",&Transform3D::translateByX,
            "translateByY",&Transform3D::translateByY,
            "translateByZ",&Transform3D::translateByZ,
            "setTranslation",static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setTranslation),
            "setTranslation",static_cast<void(Transform3D::*)(glm::vec3)>(&Transform3D::setTranslation),
            "getTranslation",&Transform3D::getTranslation,
            // Rotation =============================================================
            "getRotationX",&Transform3D::getRotationX,
            "getRotationY",&Transform3D::getRotationY,
            "getRotationZ",&Transform3D::getRotationZ,
            "setRotationX",&Transform3D::setRotationX,
            "setRotationY",&Transform3D::setRotationY,
            "setRotationZ",&Transform3D::setRotationZ,
            "rotateByX",&Transform3D::rotateByX,
            "rotateByY",&Transform3D::rotateByY,
            "rotateByZ",&Transform3D::rotateByZ,
            "setRotation",static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setRotation),
            "setRotation",static_cast<void(Transform3D::*)(glm::vec3)>(&Transform3D::setRotation),
            "getOrientation",&Transform3D::getOrientation,
            "setOrientation",static_cast<void(Transform3D::*)(float,float,float,float)>(&Transform3D::setOrientation),
            "setOrientation",static_cast<void(Transform3D::*)(glm::quat)>(&Transform3D::setOrientation),
            "setOrientationW",&Transform3D::setOrientationW,
            "getOrientationW",&Transform3D::getOrientationW,
            "setOrientationX",&Transform3D::setOrientationX,
            "getOrientationX",&Transform3D::getOrientationX,
            "setOrientationY",&Transform3D::setOrientationY,
            "getOrientationY",&Transform3D::getOrientationY,
            "setOrientationZ",&Transform3D::setOrientationZ,
            "getOrientationZ",&Transform3D::getOrientationZ,

            // Scale ================================================================
            "getScaleX",&Transform3D::getScaleX,
            "getScaleY",&Transform3D::getScaleY,
            "getScaleZ",&Transform3D::getScaleZ,
            "setScaleX",&Transform3D::setScaleX,
            "setScaleY",&Transform3D::setScaleY,
            "setScaleZ",&Transform3D::setScaleZ,
            "scaleByX",&Transform3D::scaleByX,
            "scaleByY",&Transform3D::scaleByY,
            "scaleByZ",&Transform3D::scaleByZ,
            "setScale",static_cast<void(Transform3D::*)(float,float,float)>(&Transform3D::setScale),
            "setScale",static_cast<void(Transform3D::*)(glm::vec3)>(&Transform3D::setScale)
        );
    }

    void
    LuaComponent::exposeTime
    ()
    {
        debugRegisteringClass("Time");
        sol::state_view stateView(mState);
        stateView.new_usertype<Time>("Time",
            "getCurrentFrameTime",&Time::getCurrentFrameTime,
            "getLastFrameTime",&Time::getLastFrameTime,
            "getFrameTimeDelta",&Time::getFrameTimeDelta,
            "scaleValueByFrameTime",&Time::scaleValueByFrameTime,
            "now",&Time::now,
            "nowLL",&Time::nowLL
        );

        stateView["Time"] = mProjectRuntime->getTime();
    }

    void
    LuaComponent::exposeAssimpModelInstance
    ()
    {
        debugRegisteringClass("AssimpModelInstance");
        sol::state_view stateView(mState);
        stateView.new_usertype<AssimpModelInstance>("AssimpModelInstance");
    }

    void
    LuaComponent::exposeEvent
    ()
    {
        debugRegisteringClass("Event");
        sol::state_view stateView(mState);
        stateView.new_usertype<Event>("Event",
            sol::constructors<Event(SceneObjectRuntime*,string)>(),
            "getSender",&Event::getSender,
            "getType",&Event::getType,
            "getAttribute",&Event::getAttribute,
            "getData",&Event::getData
        );
    }

    void
    LuaComponent::exposeAudioComponent
    ()
    {
        debugRegisteringClass("AudioComponent");
        sol::state_view stateView(mState);
        stateView.new_usertype<AudioComponent>("AudioComponent");
    }


    void
    LuaComponent::exposeScriptInstance
    ()
    {
        // TODO
    }

    void
    LuaComponent::exposeAudioInstance
    ()
    {
        debugRegisteringClass("AudioInstance");
        sol::state_view stateView(mState);
        stateView.new_usertype<AudioInstance>("AudioInstance",
            "getStatus",&AudioInstance::getStatus,
            "play",&AudioInstance::play,
            "pause",&AudioInstance::pause,
            "stop",&AudioInstance::stop

        );

        stateView.new_enum("AudioStatus",
            "PLAYING", AudioStatus::PLAYING,
            "PAUSED",  AudioStatus::PAUSED,
            "STOPPED", AudioStatus::STOPPED
        );
    }

    void
    LuaComponent::exposeGainput
    ()
    {
        debugRegisteringClass("Gainput");
        sol::state_view stateView(mState);
        stateView.new_usertype<gainput::InputMap>("InputMap",
            "GetBool",&gainput::InputMap::GetBool,
            "GetBoolIsNew",&gainput::InputMap::GetBoolIsNew,
            "GetBoolPrevious",&gainput::InputMap::GetBoolPrevious,
            "GetBoolWasDown",&gainput::InputMap::GetBoolWasDown,
            "GetFloat",&gainput::InputMap::GetFloat,
            "GetFloatPrevious",&gainput::InputMap::GetFloatPrevious,
            "GetFloatDelta",&gainput::InputMap::GetFloatDelta
        );

        stateView.new_enum("InputSource",
            "JS_FaceButtonNorth",   InputSource::JS_FaceButtonNorth,
            "JS_FaceButtonEast",    InputSource::JS_FaceButtonEast,
            "JS_FaceButtonWest",    InputSource::JS_FaceButtonWest,
            "JS_FaceButtonSouth",   InputSource::JS_FaceButtonSouth,
            "JS_FaceHome",          InputSource::JS_FaceHome,
            "JS_FaceStart",         InputSource::JS_FaceStart,
            "JS_FaceSelect",        InputSource::JS_FaceSelect,
            "JS_ShoulderLeft",      InputSource::JS_ShoulderLeft,
            "JS_ShoulderRight",     InputSource::JS_ShoulderRight,
            "JS_TriggerLeft",       InputSource::JS_TriggerLeft,
            "JS_TriggerRight",      InputSource::JS_TriggerRight,
            "JS_DPadNorth",         InputSource::JS_DPadNorth,
            "JS_DPadSouth",         InputSource::JS_DPadSouth,
            "JS_DPadEast",          InputSource::JS_DPadEast,
            "JS_DPadWest",          InputSource::JS_DPadWest,
            "JS_AnalogLeftStickX",  InputSource::JS_AnalogLeftStickX,
            "JS_AnalogLeftStickY",  InputSource::JS_AnalogLeftStickY,
            "JS_AnalogLeftButton",  InputSource::JS_AnalogLeftButton,
            "JS_AnalogRightStickX", InputSource::JS_AnalogRightStickX,
            "JS_AnalogRightStickY", InputSource::JS_AnalogRightStickY,
            "JS_AnalogRightButton", InputSource::JS_AnalogRightButton,
            "KB_UP",     InputSource::KB_UP,
            "KB_DOWN",   InputSource::KB_DOWN,
            "KB_LEFT",   InputSource::KB_LEFT,
            "KB_RIGHT",  InputSource::KB_RIGHT,
            "KB_SPACE",  InputSource::KB_SPACE,
            "KB_RETURN", InputSource::KB_RETURN
        );
    }

    void
    LuaComponent::removeFromScriptMap
    (SceneObjectRuntime* sceneObject)
    {
        auto log = getLog();
        map<SceneObjectRuntime*,ScriptInstance*>::iterator iter;
        sol::state_view stateView(mState);
        for(iter = begin(mScriptMap); iter != end(mScriptMap); iter++)
        {
            auto sObj = (*iter).first;
            if (sObj == sceneObject)
            {
                stateView[sObj->getUuid()] = sol::lua_nil;

                string name = sObj->getNameAndUuidString();
                log->info( "Removed script for {}" , name );

                mScriptMap.erase(iter++);
                break;
            }
        }
    }

    void
    LuaComponent::addToScriptMap
    (SceneObjectRuntime* sceneObject, ScriptInstance* script)
    {
        auto log = getLog();
        log->info(
                    "Adding {} to script map for {}",
                    script->getNameAndUuidString(),
                    sceneObject->getNameAndUuidString()
                    );

        if (createScript(sceneObject,script))
        {
            mScriptMap.insert(
                pair<SceneObjectRuntime*,ScriptInstance*>(sceneObject,script)
            );
        }
    }

    void
    LuaComponent::exposeNanoVG
    ()
    {
        /*
        debugRegisteringClass("NanoVG");
        [
                mState.new_usertype<NVGcolor>("NVGcolor"),
                mState.new_usertype<NVGpaint>("NVGpaint"),
                mState.new_usertype<NVGglyphPosition>("NVGglyphPosition"),
                mState.new_usertype<NVGtextRow>("NVGtextRow"),
                mState.new_usertype<NanoVGComponent>("NanoVG")
                .enum_("NVGsolidity")
                [
                value("NVG_SOLID",NVG_SOLID),
                value("NVG_HOLE",NVG_HOLE)
                ]
                .enum_("NVGImageFlags")
                [
                value("NVG_IMAGE_GENERATE_MIPMAPS",NVG_IMAGE_GENERATE_MIPMAPS),
                value("NVG_IMAGE_REPEATX",NVG_IMAGE_REPEATX),
                value("NVG_IMAGE_REPEATY",NVG_IMAGE_REPEATY),
                value("NVG_IMAGE_FLIPY",NVG_IMAGE_FLIPY),
                value("NVG_IMAGE_PREMULTIPLIED",NVG_IMAGE_PREMULTIPLIED),
                value("NVG_IMAGE_NEAREST",NVG_IMAGE_NEAREST)
                ]
                "BeginFrame",&NanoVGComponent::BeginFrame)
                "CancelFrame",&NanoVGComponent::CancelFrame)
                "EndFrame",&NanoVGComponent::EndFrame)
                "GlobalCompositeOperation",&NanoVGComponent::GlobalCompositeOperation)
                "GlobalCompositeBlendFunc",&NanoVGComponent::GlobalCompositeBlendFunc)
                "GlobalCompositeBlendFuncSeparate",&NanoVGComponent::GlobalCompositeBlendFuncSeparate)
                "RGB",&NanoVGComponent::RGB)
                "RGBf",&NanoVGComponent::RGBf)
                "RGBA",&NanoVGComponent::RGBA)
                "RGBAf",&NanoVGComponent::RGBAf)
                "LerpRGBA",&NanoVGComponent::LerpRGBA)
                "TransRGBA",&NanoVGComponent::TransRGBA)
                "TransRGBAf",&NanoVGComponent::TransRGBAf)
                "HSL",&NanoVGComponent::HSL)
                "HSLA",&NanoVGComponent::HSLA)
                "Save",&NanoVGComponent::Save)
                "Restore",&NanoVGComponent::Restore)
                "Reset",&NanoVGComponent::Reset)
                "ShapeAntiAlias",&NanoVGComponent::ShapeAntiAlias)
                "StrokeColor",&NanoVGComponent::StrokeColor)
                "StrokePaint",&NanoVGComponent::StrokePaint)
                "FillColor",&NanoVGComponent::FillColor)
                "FillPaint",&NanoVGComponent::FillPaint)
                "MiterLimit",&NanoVGComponent::MiterLimit)
                "StrokeWidth",&NanoVGComponent::StrokeWidth)
                "LineCap",&NanoVGComponent::LineCap)
                "LineJoin",&NanoVGComponent::LineJoin)
                "GlobalAlpha",&NanoVGComponent::GlobalAlpha)
                "ResetTransform",&NanoVGComponent::ResetTransform)
                "Transform",&NanoVGComponent::Transform)
                "Translate",&NanoVGComponent::Translate)
                "Rotate",&NanoVGComponent::Rotate)
                "SkewX",&NanoVGComponent::SkewX)
                "SkewY",&NanoVGComponent::SkewY)
                "Scale;",&NanoVGComponent::Scale)
                "CurrentTransform",&NanoVGComponent::CurrentTransform)
                "TransformIdentity",&NanoVGComponent::TransformIdentity)
                "TransformTranslate",&NanoVGComponent::TransformTranslate)
                "TransformScale",&NanoVGComponent::TransformScale)
                "TransformRotate",&NanoVGComponent::TransformRotate)
                "TransformSkewX",&NanoVGComponent::TransformSkewX)
                "TransformSkewY",&NanoVGComponent::TransformSkewY)
                "TransformMultiply",&NanoVGComponent::TransformMultiply)
                "TransformPremultiply",&NanoVGComponent::TransformPremultiply)
                "TransformInverse",&NanoVGComponent::TransformInverse)
                "TransformPoint",&NanoVGComponent::TransformPoint)
                "DegToRad",&NanoVGComponent::DegToRad)
                "RadToDeg",&NanoVGComponent::RadToDeg)
                "CreateImage",&NanoVGComponent::CreateImage)
                "CreateImageMem",&NanoVGComponent::CreateImageMem)
                "CreateImageRGBA",&NanoVGComponent::CreateImageRGBA)
                "UpdateImage",&NanoVGComponent::UpdateImage)
                "ImageSize",&NanoVGComponent::ImageSize)
                "DeleteImage",&NanoVGComponent::DeleteImage)
                "LinearGradient",&NanoVGComponent::LinearGradient)
                "BoxGradient",&NanoVGComponent::BoxGradient)
                "RadialGradient",&NanoVGComponent::RadialGradient)
                "ImagePattern",&NanoVGComponent::ImagePattern)
                "Scissor",&NanoVGComponent::Scissor)
                "IntersectScissor",&NanoVGComponent::IntersectScissor)
                "ResetScissor",&NanoVGComponent::ResetScissor)
                "BeginPath",&NanoVGComponent::BeginPath)
                "MoveTo",&NanoVGComponent::MoveTo)
                "LineTo",&NanoVGComponent::LineTo)
                "BezierTo",&NanoVGComponent::BezierTo)
                "QuadTo",&NanoVGComponent::QuadTo)
                "ArcTo",&NanoVGComponent::ArcTo)
                "ClosePath",&NanoVGComponent::ClosePath)
                "PathWinding",&NanoVGComponent::PathWinding)
                "Arc",&NanoVGComponent::Arc)
                "Rect",&NanoVGComponent::Rect)
                "RoundedRect",&NanoVGComponent::RoundedRect)
                "RoundedRectVarying",&NanoVGComponent::RoundedRectVarying)
                "Ellipse",&NanoVGComponent::Ellipse)
                "Circle",&NanoVGComponent::Circle)
                "Fill",&NanoVGComponent::Fill)
                "Stroke",&NanoVGComponent::Stroke)
                "CreateFont",&NanoVGComponent::CreateFont)
                "CreateFontMem",&NanoVGComponent::CreateFontMem)
                "FindFont",&NanoVGComponent::FindFont)
                "AddFallbackFontId",&NanoVGComponent::AddFallbackFontId)
                "AddFallbackFont",&NanoVGComponent::AddFallbackFont)
                "FontSize",&NanoVGComponent::FontSize)
                "FontBlur",&NanoVGComponent::FontBlur)
                "TextLetterSpacing",&NanoVGComponent::TextLetterSpacing)
                "TextLineHeight",&NanoVGComponent::TextLineHeight)
                "TextAlign",&NanoVGComponent::TextAlign)
                "FontFaceId",&NanoVGComponent::FontFaceId)
                "FontFace",&NanoVGComponent::FontFace)
                "Text",&NanoVGComponent::Text)
                "TextBox",&NanoVGComponent::TextBox)
                "TextBounds",&NanoVGComponent::TextBounds)
                "TextBoxBounds",&NanoVGComponent::TextBoxBounds)
                "TextGlyphPositions",&NanoVGComponent::TextGlyphPositions)
                "TextMetrics",&NanoVGComponent::TextMetrics)
                "TextBreakLines",&NanoVGComponent::TextBreakLines)
                */
    }

    void LuaComponent::exposeGLM()
    {
        debugRegisteringClass("GLM");
        sol::state_view stateView(mState);

        stateView.new_usertype<glm::vec3>("vec3",
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
        );
        stateView.new_usertype<glm::quat>("quat",
            "w", &glm::quat::w,
            "x", &glm::quat::x,
            "y", &glm::quat::y,
            "z", &glm::quat::z
        );

        stateView.new_usertype<glm::mat4>("mat4");
    }

    void
    LuaComponent::setInputMap
    (gainput::InputMap* map)
    {
        mInputMap = map;
    }

    // API Exposure Methods ======================================================

    void
    LuaComponent::debugRegisteringClass
    (string className)
    {
        auto log = getLog();
        log->info( "Registering Class {}",  className );
        return;
    }

    void
    LuaComponent::exposeAPI
    ()
    {
        // Runtimes
        exposeProjectRuntime();
        exposeSceneObjectRuntime();
        // Dream Misc
        exposeEvent();
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

