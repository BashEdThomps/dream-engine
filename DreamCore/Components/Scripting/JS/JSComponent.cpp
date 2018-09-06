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

#include <sstream>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

#include "LuaScriptInstance.h"

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
using luabind::bases;

int
errorr
(lua_State *L)
{
    // log the error message
    object msg(from_stack( L, -1 ));
    stringstream ss;
    ss << msg;
    cerr << "RuntimeError: " << ss.str() << endl;
    // log the callstack
    string traceback = call_function<string>( globals(L)["debug"]["traceback"] );
    cerr << "TraceBack: " << traceback.c_str();
    return 0;
}

namespace Dream
{
    LuaComponent::LuaComponent
    (shared_ptr<ProjectRuntime> project, shared_ptr<LuaScriptCache> cache)
        : IComponent(),
          mScriptCache(cache),
          mProjectRuntime(project),
          mState(nullptr)
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

        if (mState != nullptr)
        {
            lua_close(mState);
        }

        mScriptMap.clear();
    }

    bool
    LuaComponent::init
    ()
    {
        auto log = getLog();
        log->info( "Initialising LuaComponent" );
        mState = luaL_newstate();
        log->info( "got a state" );
        if (mState != nullptr)
        {
            luabind::open(mState);
            log->info( "opened" );
            luaopen_base(mState);
            log->info( "opened base" );
            set_pcall_callback(&errorr);
            log->info( "set error callback" );
            luaL_dostring(mState, mScriptLoadFromString.c_str());
            log->info( "loaded load string" );
            exposeAPI();
            return true;
        }
        else
        {
            log->error( "Error creating lua state, LuaComponent::mState == nullptr");
        }
        return false;
    }

    bool
    LuaComponent::createScript
    (shared_ptr<SceneObjectRuntime> sceneObject, shared_ptr<LuaScriptInstance> luaScript)
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
    LuaComponent::loadScript
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = sceneObject->getScriptInstance();

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
            string script = mScriptCache->getScript(path);

            log->info( "calling scriptLoadFromString in lua for {}" , id );

            bool result = call_function<bool>(mState, "scriptLoadFromString", newScriptTable, script.c_str());

            if (result)
            {
                log->info("Loaded Script Successfully");
                object reg = registry(mState);
                reg[id] = newScriptTable;
            }
            else
            {
                log->info("Loaded Script Failed");
                scriptInstance->setError(true);
                return false;
            }
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
    LuaComponent::stackDump
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

    void
    LuaComponent::updateComponent
    ()
    {
        while(mRunning)
        {
            if (mShouldUpdate && mActiveSceneRuntime != nullptr)
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

                if (!mParallel) break;
            }
            if (mParallel) std::this_thread::yield();
        }
    }

    bool
    LuaComponent::updateNanoVG
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

    bool
    LuaComponent::executeScriptUpdate
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = sceneObject->getScriptInstance();

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
    LuaComponent::executeScriptNanoVG
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = sceneObject->getScriptInstance();

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
    LuaComponent::executeScriptInit
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = sceneObject->getScriptInstance();

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
    LuaComponent::executeScriptInput
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = sceneObject->getScriptInstance();

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
                call_function<void>(funq,sceneObject,mInputMap);
            }
            else
            {
                log->error( "Attempted to call onInput on invalid function.");
            }
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
    LuaComponent::executeScriptEvent
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        string id = sceneObject->getUuid();
        shared_ptr<LuaScriptInstance> scriptInstance = sceneObject->getScriptInstance();

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
    LuaComponent::setInputMap
    (gainput::InputMap *map)
    {
        mInputMap = map;
    }

    // API Exposure Methods ======================================================

    void
    LuaComponent::exposeDreamBase
    ()
    {
        debugRegisteringClass("Dream base classes");
        module(mState)
        [
            class_<DreamObject, shared_ptr<DreamObject>>("DreamObject"),
            class_<IDefinition, DreamObject, shared_ptr<DreamObject>>("IDefinition"),
            class_<IRuntime, DreamObject, shared_ptr<DreamObject>>("IRuntime"),
            class_<IComponent, DreamObject, shared_ptr<DreamObject>>("IComponent"),
            class_<IAssetDefinition, DreamObject, shared_ptr<DreamObject>>("IAssetDefinition"),
            class_<IAssetInstance, DreamObject, shared_ptr<DreamObject>>("IAssetInstance"),
            class_<IWindowComponent, bases<DreamObject, IComponent>, shared_ptr<IComponent>>("IWindowComponent")
                .def("getWidth",&IWindowComponent::getWidth)
                .def("getHeight",&IWindowComponent::getHeight)
                .def("setShouldClose",&IWindowComponent::setShouldClose)
                .def("getMouseX",&IWindowComponent::getMouseX)
                .def("getMouseY",&IWindowComponent::getMouseY)
        ];
    }

    void
    LuaComponent::exposeProjectRuntime
    ()
    {
        debugRegisteringClass("ProjectRuntime");
        module(mState)
        [
            class_<ProjectRuntime, bases<DreamObject,IRuntime>, shared_ptr<IRuntime>>("ProjectRuntime")
                .def("getAudioComponent",&ProjectRuntime::getAudioComponent)
                .def("getGraphicsComponent",&ProjectRuntime::getGraphicsComponent)
                .def("getNanoVGComponent",&ProjectRuntime::getNanoVGComponent)
                .def("getPhysicsComponent",&ProjectRuntime::getPhysicsComponent)
                .def("getWindowComponent",&ProjectRuntime::getWindowComponent)
                .def("getTime",&ProjectRuntime::getTime)
                .def("getCamera",&ProjectRuntime::getCamera)
        ];
        globals(mState)["Runtime"] = mProjectRuntime.get();
    }

    void
    LuaComponent::exposeCamera
    ()
    {
        module(mState)
        [
            luabind::class_<Camera, DreamObject, shared_ptr<DreamObject>>("Camera")
                .def("processKeyboard",&Camera::processKeyboard)
                .def("processMouseMovement",&Camera::processMouseMovement)
                .def("pan",&Camera::pan)
                .def("flyForward",&Camera::flyForward)
                .def("flyBackward",&Camera::flyBackward)
                .def("flyLeft",&Camera::flyLeft)
                .def("flyRight",&Camera::flyRight)
                .def("flyUp",&Camera::flyUp)
                .def("flyDown",&Camera::flyDown)
                .def("flyX",&Camera::flyX)
                .def("flyY",&Camera::flyY)
                .def("flyZ",&Camera::flyZ)
                .def("setFreeMode",&Camera::setFreeMode)
                .def("setLookAt",static_cast<void(Camera::*)(float,float,float)>(&Camera::setLookAt))
                .def("setLookAt",static_cast<void(Camera::*)(vec3)>( &Camera::setLookAt))
                .def("setTranslation",static_cast<void(Camera::*)(float,float,float)>(&Camera::setTranslation))
                .def("orbit",&Camera::orbit)

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
    LuaComponent::debugRegisteringClass
    (string className)
    {
        auto log = getLog();
        log->info( "Registering Class {}",  className );
        return;
    }

    void
    LuaComponent::exposePathComponent
    ()
    {
        debugRegisteringClass("PathComponent");
        module(mState)
        [
            class_<PathComponent, bases<DreamObject, IComponent>, shared_ptr<IComponent>>("PathComponent")
        ];
    }

    void
    LuaComponent::exposePathInstance
    ()
    {
        debugRegisteringClass("PathInstance");
        module(mState)
        [
        class_<PathInstance, bases<DreamObject, IAssetInstance>, shared_ptr<IAssetInstance>>("PathInstance")
            .def("generate",&PathInstance::generate)
            .def("getSplinePoints",&PathInstance::getSplinePoints)
            .def("getSplinePoint",&PathInstance::getSplinePoint)
            .def("getUStep",&PathInstance::getUStep)
            .def("setUStep",&PathInstance::setUStep)
            .def("stepPath",&PathInstance::stepPath)
        ];
    }

    void
    LuaComponent::exposeFontInstance
    ()
    {
        debugRegisteringClass("FontInstance");
        module(mState)
        [
            class_<FontInstance, bases<DreamObject, IAssetInstance>, shared_ptr<IAssetInstance>>("FontInstance")
            .def("setText",&FontInstance::setText)
            .def("getText",&FontInstance::getText)
        ];
    }

    void
    LuaComponent::exposeGraphicsComponent
    ()
    {
        debugRegisteringClass("GraphicsComponent");
        module(mState)
        [
            class_<GraphicsComponent, bases<DreamObject, IComponent>, shared_ptr<IComponent>>("GraphicsComponent")
        ];
    }

    void
    LuaComponent::exposeLightInstance
    ()
    {
        debugRegisteringClass("LightInstance");
        module(mState)
        [
            class_<LightInstance, bases<DreamObject, IAssetInstance>, shared_ptr<IAssetInstance>>("LightInstance")
            // TODO
        ];
    }

    void
    LuaComponent::exposeShaderInstance
    ()
    {
        debugRegisteringClass("ShaderInstance");
        module(mState)
        [
            class_<ShaderInstance, bases<DreamObject, IAssetInstance>, shared_ptr<IAssetInstance>>("ShaderInstance")
                .def("getUuid", &ShaderInstance::getUuid)
        ];

        debugRegisteringClass("ShaderUniform");
        module(mState)
        [
            class_<ShaderUniform, DreamObject, shared_ptr<DreamObject>>("ShaderUniform")
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
        ];
    }

    void
    LuaComponent::exposeSpriteInstance
    ()
    {
        debugRegisteringClass("SpriteInstance");
        module(mState)
        [
            class_<SpriteInstance, bases<DreamObject, IAssetInstance>, shared_ptr<IAssetInstance>>("SpriteInstance")
        ];
    }

    void
    LuaComponent::exposePhysicsComponent
    ()
    {
        debugRegisteringClass("PhysicsComponent");
        module(mState)
        [
            class_<PhysicsComponent, bases<DreamObject, IComponent>, shared_ptr<IComponent>>("PhysicsComponent")
                .def("setDebug",&PhysicsComponent::setDebug)
        ];
    }

    void
    LuaComponent::exposePhysicsObjectInstance
    ()
    {
        debugRegisteringClass("PhysicsObjectInstance");
        module(mState)
        [
            class_<PhysicsObjectInstance, bases<DreamObject, IAssetInstance>, shared_ptr<IAssetInstance>>("PhysicsObjectInstance")
                .def("getUuid", &PhysicsObjectInstance::getUuid)
                .def("setLinearVelocity", &PhysicsObjectInstance::setLinearVelocity)
        ];
    }

    void
    LuaComponent::exposeMath
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
                    luabind::def("sin",&Math::_sinf),
                    luabind::def("cos",&Math::_cosf),
                    luabind::def("sqrt",&Math::_sqrtf)
                ]
        ];
    }

    void
    LuaComponent::exposeSceneObjectRuntime
    ()
    {
        debugRegisteringClass("SceneObjectRuntime");
        module(mState)
        [
            class_<SceneObjectRuntime, bases<DreamObject, IRuntime>, shared_ptr<DreamObject>>("DreamObject")

                .def("getChildByUuid",&SceneObjectRuntime::getChildRuntimeByUuid)

                .def("getParent",&SceneObjectRuntime::getParentRuntime)
                .def("setParent",&SceneObjectRuntime::setParentRuntime)

                .def("getTransform",&SceneObjectRuntime::getTransform)
                .def("setTransform",&SceneObjectRuntime::setTransform)

                .def("addAssetDefinitionUuidToLoad",&SceneObjectRuntime::addAssetDefinitionUuidToLoad)

                .def("walk",&SceneObjectRuntime::walk)
                .def("getPath",&SceneObjectRuntime::getPathInstance)
                .def("getAudio",&SceneObjectRuntime::getAudioInstance)
                .def("getSprite",&SceneObjectRuntime::getSpriteInstance)
                .def("getModel",&SceneObjectRuntime::getModelInstance)
                .def("getShader",&SceneObjectRuntime::getShaderInstance)
                .def("getLight",&SceneObjectRuntime::getLightInstance)
                .def("getFont",&SceneObjectRuntime::getFontInstance)
                .def("getPhysicsObject",&SceneObjectRuntime::getPhysicsObjectInstance)

                .def("hasPath",&SceneObjectRuntime::hasPathInstance)
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
    LuaComponent::exposeTransform3D
    ()
    {
        debugRegisteringClass("Transform3D");
        module(mState) [
            class_<Transform3D, DreamObject, shared_ptr<DreamObject>>("Transform3D")
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
                .def("setTranslation",static_cast<void(Transform3D::*)(glm::vec3)>(&Transform3D::setTranslation))
                .def("getTranslation",&Transform3D::getTranslation)
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
                .def("setRotation",static_cast<void(Transform3D::*)(glm::vec3)>(&Transform3D::setRotation))
                .def("getOrientation",&Transform3D::getOrientation)
                .def("setOrientation",static_cast<void(Transform3D::*)(float,float,float,float)>(&Transform3D::setOrientation))
                .def("setOrientation",static_cast<void(Transform3D::*)(glm::quat)>(&Transform3D::setOrientation))
                .def("setOrientationW",&Transform3D::setOrientationW)
                .def("getOrientationW",&Transform3D::getOrientationW)
                .def("setOrientationX",&Transform3D::setOrientationX)
                .def("getOrientationX",&Transform3D::getOrientationX)
                .def("setOrientationY",&Transform3D::setOrientationY)
                .def("getOrientationY",&Transform3D::getOrientationY)
                .def("setOrientationZ",&Transform3D::setOrientationZ)
                .def("getOrientationZ",&Transform3D::getOrientationZ)

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
                .def("setScale",static_cast<void(Transform3D::*)(glm::vec3)>(&Transform3D::setScale))
            ];
    }

    void
    LuaComponent::exposeTime
    ()
    {
        debugRegisteringClass("Time");
        module(mState)
        [
            class_<Time, DreamObject, shared_ptr<Time>>("Time")
                .def("getCurrentFrameTime",&Time::getCurrentFrameTime)
                .def("getLastFrameTime",&Time::getLastFrameTime)
                .def("getFrameTimeDelta",&Time::getFrameTimeDelta)
                .def("scaleValueByFrameTime",&Time::scaleValueByFrameTime)
                .def("now",&Time::now)
                .def("nowLL",&Time::nowLL)
        ];
    }

    void
    LuaComponent::exposeAssimpModelInstance
    ()
    {
        debugRegisteringClass("AssimpModelInstance");
        module(mState)
        [
            class_<AssimpModelInstance, bases<DreamObject, IAssetInstance>, shared_ptr<IAssetInstance>>("AssimpModelInstance")
        ];
    }

    void
    LuaComponent::exposeEvent
    ()
    {
        debugRegisteringClass("Event");
        module(mState)
        [
            class_<Event, DreamObject, shared_ptr<DreamObject>>("Event")
                .def("getSender",&Event::getSender)
                .def("getType",&Event::getType)
                .def("getAttribute",&Event::getAttribute)
                .def("setAttribute",&Event::setAttribute)
        ];
    }

    void
    LuaComponent::exposeAudioComponent
    ()
    {
        debugRegisteringClass("AudioComponent");
        module(mState)
        [
            class_<AudioComponent, bases<DreamObject, IComponent>, shared_ptr<IComponent>>("AudioComponent")
                .def("play",&AudioComponent::playAudioAsset)
                .def("pause",&AudioComponent::pauseAudioAsset)
                .def("stop",&AudioComponent::stopAudioAsset)
        ];
    }


    void
    LuaComponent::exposeLuaScriptInstance
    ()
    {
        // TODO
    }

    void
    LuaComponent::exposeAudioInstance
    ()
    {
        debugRegisteringClass("AudioInstance");
        module(mState)
        [
            class_<AudioInstance, bases<DreamObject, IAssetInstance>, shared_ptr<AudioInstance>>("AudioInstance")
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
    LuaComponent::exposeGainput
    ()
    {
        debugRegisteringClass("Gainput");
        module(mState)
        [
                class_<gainput::InputMap>("InputMap")
                    .def("GetBool",&gainput::InputMap::GetBool)
                    .def("GetBoolIsNew",&gainput::InputMap::GetBoolIsNew)
                    .def("GetBoolPrevious",&gainput::InputMap::GetBoolPrevious)
                    .def("GetBoolWasDown",&gainput::InputMap::GetBoolWasDown)
                    .def("GetFloat",&gainput::InputMap::GetFloat)
                    .def("GetFloatPrevious",&gainput::InputMap::GetFloatPrevious)
                    .def("GetFloatDelta",&gainput::InputMap::GetFloatDelta)
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
    }

    void
    LuaComponent::exposeAPI
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
        exposeLuaScriptInstance();
        // Physics
        exposePhysicsComponent();
        exposePhysicsObjectInstance();
    }

    void
    LuaComponent::removeFromScriptMap
    (shared_ptr<SceneObjectRuntime> sceneObject)
    {
        auto log = getLog();
        map<shared_ptr<SceneObjectRuntime>,shared_ptr<LuaScriptInstance>>::iterator iter;
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
    LuaComponent::addToScriptMap
    (shared_ptr<SceneObjectRuntime> sceneObject, shared_ptr<LuaScriptInstance> script)
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
                pair<shared_ptr<SceneObjectRuntime>,shared_ptr<LuaScriptInstance>>
                (sceneObject,script)
            );
        }
    }

    void
    LuaComponent::exposeNanoVG
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
                .enum_("NVGImageFlags")
                [
                value("NVG_IMAGE_GENERATE_MIPMAPS",NVG_IMAGE_GENERATE_MIPMAPS),
                value("NVG_IMAGE_REPEATX",NVG_IMAGE_REPEATX),
                value("NVG_IMAGE_REPEATY",NVG_IMAGE_REPEATY),
                value("NVG_IMAGE_FLIPY",NVG_IMAGE_FLIPY),
                value("NVG_IMAGE_PREMULTIPLIED",NVG_IMAGE_PREMULTIPLIED),
                value("NVG_IMAGE_NEAREST",NVG_IMAGE_NEAREST)
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

    void LuaComponent::exposeGLM()
    {
        debugRegisteringClass("GLM");
        module(mState)
        [
            class_<glm::vec3>("vec3")
                .def_readwrite("x",&glm::vec3::x)
                .def_readwrite("y",&glm::vec3::y)
                .def_readwrite("z",&glm::vec3::z),
            class_<glm::quat>("quat")
                .def_readwrite("w",&glm::quat::w)
                .def_readwrite("x",&glm::quat::x)
                .def_readwrite("y",&glm::quat::y)
                .def_readwrite("z",&glm::quat::z),
            class_<glm::mat4>("mat4")
        ];
    }

} // End of Dream

