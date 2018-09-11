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
#include "../../../Utilities/Math.h"
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
    (shared_ptr<ProjectRuntime> project, shared_ptr<ScriptCache> cache)
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
    }

    bool
    LuaComponent::init
    ()
    {
        auto log = getLog();
        log->info( "Initialising LuaComponent" );
        mState = luaL_newstate();
        //luaL_openlibs(mState);
        sol::state_view sView(mState);
        sView.open_libraries(sol::lib::base, sol::lib::package);
        log->info( "Got a sol state" );
        exposeAPI();
        return false;
    }

    /*
     * Instanciate the script and put into global registry
     */
    bool
    LuaComponent::loadScript
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();

        log->info( "loadScript called for {}", id );

        shared_ptr<LuaScriptInstance> scriptInstance = dynamic_pointer_cast<LuaScriptInstance>(sceneObject->getScriptInstance());

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
           log->error("Could not execute lua script: {}",what);
           scriptInstance->setError(true);
           return false;
        }

        sol::function onInitFunction = solStateView[sceneObject->getUuid()][Constants::LUA_INIT_FUNCTION];

        auto initResult = onInitFunction(sceneObject);

        if (!initResult.valid())
        {
            // An error has occured
           sol::error err = initResult;
           std::string what = err.what();
           log->error("Could not execute onInit in lua script: {}",what);
           scriptInstance->setError(true);
           return false;
        }

        /*
        sol::function onInputFunction  = solStateView[Constants::LUA_INPUT_FUNCTION];
        sol::function onEventFunction  = solStateView[Constants::LUA_EVENT_FUNCTION];
        sol::function onUpdateFunction = solStateView[Constants::LUA_UPDATE_FUNCTION];
        sol::function onNanoVGFunction = solStateView[Constants::LUA_NANOVG_FUNCTION];
        */

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
    LuaComponent::updateNanoVG
    ()
    {
        auto log = getLog();
        log->info("UpdateNanoVG Called");

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

    bool
    LuaComponent::executeScriptUpdate
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = dynamic_pointer_cast<LuaScriptInstance>(sceneObject->getScriptInstance());

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
           log->error("Could not execute onUpdate in lua script: {}",what);
           scriptInstance->setError(true);
           return false;
        }

        return true;
    }

    bool
    LuaComponent::executeScriptNanoVG
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = dynamic_pointer_cast<LuaScriptInstance>(sceneObject->getScriptInstance());

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
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = dynamic_pointer_cast<LuaScriptInstance>(sceneObject->getScriptInstance());

        if (scriptInstance->getError() )
        {
            log->error("Cannot execute {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info("Calling onInit in {} for {}",  scriptInstance->getName(),  sceneObject->getName());

        return true;
    }

    bool
    LuaComponent::executeScriptInput
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = dynamic_pointer_cast<LuaScriptInstance>(sceneObject->getScriptInstance());

        if (scriptInstance->getError())
        {
            log->error("Cannot execute {} in error state", scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info("Calling onInput for {} (Has Focus) {}", sceneObject->getNameAndUuidString());

        //scriptInstance->callOnInputFunction(mInputMap);

        return true;
    }

    bool
    LuaComponent::executeScriptEvent
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = dynamic_pointer_cast<LuaScriptInstance>(sceneObject->getScriptInstance());

        if (scriptInstance->getError())
        {
            log->error( "Cannot execute {} in error state ",  scriptInstance->getNameAndUuidString());
            return false;
        }

        log->info( "Calling onEvent for {}", sceneObject->getNameAndUuidString());

        //scriptInstance->callOnEventFunction(sceneObject->getEventQueue());

        return true;
    }

    // API Exposure Methods ======================================================

    void
    LuaComponent::exposeDreamBase
    ()
    {
        /*
        debugRegisteringClass("Dream base classes");
        stateView.new_usertype<Dream::DreamObject>("DreamObject");
        stateView.new_usertype<Dream::IDefinition>("IDefinition");
        stateView.new_usertype<IRuntime>("IRuntime");
        stateView.new_usertype<IComponent>("IComponent");
        stateView.new_usertype<IAssetDefinition>("IAssetDefinition");
        stateView.new_usertype<IAssetInstance>("IAssetInstance");
        stateView.new_usertype<IWindowComponent>("IWindowComponent",
            "getWidth",       &IWindowComponent::getWidth,
            "getHeight",      &IWindowComponent::getHeight,
            "setShouldClose", &IWindowComponent::setShouldClose,
            "getMouseX",      &IWindowComponent::getMouseX,
            "getMouseY",      &IWindowComponent::getMouseY
        );
        */
    }

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
        stateView["Runtime"] = mProjectRuntime.get();
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
            "orbit",&Camera::orbit
        );

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
            "setLinearVelocity", &PhysicsObjectInstance::setLinearVelocity
        );
    }

    void
    LuaComponent::exposeMath
    ()
    {
        debugRegisteringClass("Math");
        sol::state_view stateView(mState);
        stateView.new_usertype<Math>("Math",
            "degreesToRadians",&Math::degreesToRadians,
            "radiansToDegrees",&Math::radiansToDegrees,
            "pow",&Math::_pow,
            "sin",&Math::_sinf,
            "cos",&Math::_cosf,
            "sqrt",&Math::_sqrtf
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
            "getChildByUuid",&SceneObjectRuntime::getChildRuntimeByUuid,
            "getParent",&SceneObjectRuntime::getParentRuntime,
            "setParent",&SceneObjectRuntime::setParentRuntime,
            "getTransform",&SceneObjectRuntime::getTransform,
            "setTransform",&SceneObjectRuntime::setTransform,
            "addAssetDefinitionUuidToLoad",&SceneObjectRuntime::addAssetDefinitionUuidToLoad,
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
            "hasPhysicsObject",&SceneObjectRuntime::hasPhysicsObjectInstance
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
            "getSender",&Event::getSender,
            "getType",&Event::getType,
            "getAttribute",&Event::getAttribute,
            "setAttribute",&Event::setAttribute
        );
    }

    void
    LuaComponent::exposeAudioComponent
    ()
    {
        debugRegisteringClass("AudioComponent");
        sol::state_view stateView(mState);
        stateView.new_usertype<AudioComponent>("AudioComponent",
            "play",&AudioComponent::playAudioAsset,
            "pause",&AudioComponent::pauseAudioAsset,
            "stop",&AudioComponent::stopAudioAsset
        );
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
            "getStatus",&AudioInstance::getStatus
        );
                /*
            mState.new_usertype<AudioStatus>("AudioStatus")
                .enum_("AudioStatus")
                [
                    value("PLAYING", AudioStatus::PLAYING),
                    value("PAUSED",  AudioStatus::PAUSED),
                    value("STOPPED", AudioStatus::STOPPED)
                ]
                        */
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
        /*
                    .enum_("InputSource")
                    [
                        value("FaceButtonNorth", InputSource::FaceButtonNorth),
                        value("FaceButtonEast", InputSource::FaceButtonEast),
                        value("FaceButtonWest", InputSource::FaceButtonWest),
                        value("FaceButtonSouth", InputSource::FaceButtonSouth),

                        value("FaceHome", InputSource::FaceHome),
                        value("FaceStart", InputSource::FaceStart),
                        value("FaceSelect", InputSource::FaceSelect),

                        value("ShoulderLeft", InputSource::ShoulderLeft),
                        value("ShoulderRight", InputSource::ShoulderRight),

                        value("TriggerLeft", InputSource::TriggerLeft),
                        value("TriggerRight", InputSource::TriggerRight),

                        value("DPadNorth", InputSource::DPadNorth),
                        value("DPadSouth", InputSource::DPadSouth),
                        value("DPadEast", InputSource::DPadEast),
                        value("DPadWest", InputSource::DPadWest),

                        value("AnalogLeftStickX", InputSource::AnalogLeftStickX),
                        value("AnalogLeftStickY", InputSource::AnalogLeftStickY),
                        value("AnalogLeftButton", InputSource::AnalogLeftButton),

                        value("AnalogRightStickX", InputSource::AnalogRightStickX),
                        value("AnalogRightStickY", InputSource::AnalogRightStickY),
                        value("AnalogRightButton", InputSource::AnalogRightButton)
                    ]
        ];
        */
    }

    void
    LuaComponent::removeFromScriptMap
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        map<shared_ptr<SceneObjectRuntime>,shared_ptr<ScriptInstance>>::iterator iter;
        for(iter = begin(mScriptMap); iter != end(mScriptMap); iter++)
        {
            if ((*iter).first == sceneObject)
            {
                /*
                string id = (*iter).first->getUuid();
                object reg = registry(mState);
                reg[id] = nil;
                */

                string name = (*iter).first->getNameAndUuidString();
                log->info( "Removed script for {}" , name );

                mScriptMap.erase(iter++);
                break;
            }
        }
    }

    void
    LuaComponent::addToScriptMap
    (shared_ptr<SceneObjectRuntime> sceneObject, shared_ptr<ScriptInstance> script)
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
                pair<
                    shared_ptr<SceneObjectRuntime>,
                    shared_ptr<ScriptInstance>
                >(sceneObject,script)
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

} // End of Dream

