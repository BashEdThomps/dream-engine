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

#include <sstream>
#include <SDL2/SDL_keycode.h>
#include <luabind/adopt_policy.hpp>
#include "LuaComponent.h"
#include "LuaScriptCache.h"
#include "../../Event.h"
#include "../../Math.h"

namespace Dream {

    LuaComponent::LuaComponent() {
        mState = nullptr;
    }

    LuaComponent::~LuaComponent() {
        if (DEBUG) {
            cout << "LuaComponent: Destroying Object" << endl;
        }

        if (mState != nullptr) {
            lua_close(mState);
        }
    }

    bool LuaComponent::init() {
        if (DEBUG) {
            cout << "LuaComponent: Initialising LuaComponent" << endl;
        }
        mState = luaL_newstate();
        luabind::open(mState);
        luaopen_base(mState);
        luabind::set_pcall_callback(&errorHandler);
        luaL_dostring(mState, mScriptLoadFromFile.c_str());
        luaL_dostring(mState, mScriptLoadFromString.c_str());
        bindTime();
        bindDreamEngine();
        bindMath();
        bindComponents();
        bindAssetClasses();
        bindProject();
        bindSceneObject();
        bindScene();
        bindTransform3D();
        bindDreamEvent();
        bindSDL();
        bindGameController();
        bindCamera();
        return mState != nullptr;
    }

    bool LuaComponent::createAllScripts() {
        if (VERBOSE) {
            cout << "LuaComponent: CreateAllScripts Called" << endl;
        }
        map<SceneObject*,LuaScriptInstance*>::iterator scriptIt;
        for (scriptIt=mScriptMap->begin(); scriptIt != mScriptMap->end(); scriptIt++) {

            SceneObject *sceneObject = scriptIt->first;
            LuaScriptInstance *luaScript = scriptIt->second;

            if (luaScript == nullptr) {
                cerr << "LuaComponent: Load Failed, LuaScriptInstance is NULL" << endl;
                continue;
            }

            if (sceneObject == nullptr) {
                cerr << "LuaComponent: Load Failed, SceneObject is NULL" << endl;
                continue;
            }

            if (luaScript->getLoadedFlag()) {
                continue;
            }

            if (sceneObject->getDeleteFlag()) {

                if (DEBUG) {
                    cout << "LuaComponent: SceneObject marked for deletion, not loading script." << endl;
                }
                continue;
            }

            if (DEBUG) {
                cout << "LuaComponent: loading script '" << luaScript->getName() << "' for '" << sceneObject->getName() << "'" << endl;
                cout << "LuaComponent: Loading Lua script from " << luaScript->getAbsolutePath()
                     << endl << flush;
            }

            if (!loadScript(sceneObject, luaScript)) {
                return false;
            }

            if (DEBUG) {
                cout << "LuaComponent: Loaded " << sceneObject->getUuid() << " Successfully" << endl;
            }

            luaScript->setLoadedFlag(true);
            executeScriptInit(sceneObject);
        }

        return true;
    }

    bool LuaComponent::loadScript(SceneObject* sceneObject, LuaScriptInstance* scriptInstance) {
        string id = sceneObject->getUuid();
        if (DEBUG) {
            cout << "LuaComponent: loadScript called for " << id << endl;
        }
        luabind::object newtable = luabind::newtable(mState);
        try {
            string path = scriptInstance->getAbsolutePath();
            string script = LuaScriptCache::getScript(path);

            // Old way
            //luabind::call_function<void>(mState, "scriptLoadFromFile", newtable, path.c_str());

            // With Cache...
            luabind::call_function<void>(mState, "scriptLoadFromString", newtable, script.c_str());

            luabind::object reg = luabind::registry(mState);
            reg[id] = newtable;
        } catch (luabind::error e) {
            cerr << "LuaComponent: loadScript exception:" << endl
                 << "\t" << e.what() << endl;
            return false;
        }
        return true;
    }

    void LuaComponent::bindDreamEngine() {
        luabind::module(mState) [
                luabind::class_<DreamEngine>("DreamEngine")
                .def("getActiveScene",&DreamEngine::getActiveScene)
                .def("getTime",&DreamEngine::getTime)
                .def("loadSceneByUuid",&DreamEngine::loadSceneByUuid)
                .def("getGraphicsComponent",&DreamEngine::getGraphicsComponent)
                .def("getPhysicsComponent",&DreamEngine::getPhysicsComponent)
                .def("getCamera",&DreamEngine::getCamera)
                .def("getGameController",&DreamEngine::getGameController)
                .scope [
                luabind::def("getInstance",&DreamEngine::getInstance),
                luabind::def("setDebug",&dreamSetDebug),
                luabind::def("setVerbose",&dreamSetVerbose)
                ]
                ];
    }

    void LuaComponent::bindMath() {
        luabind::module(mState) [
                luabind::def("degreesToRadians",&Math::degreesToRadians),
                luabind::def("radiansToDegrees",&Math::radiansToDegrees)
                ];
    }

    void LuaComponent::bindComponents() {
        bindAnimationComponent();
        bindAudioComponent();
        bindGraphicsComponent();
        bindPhysicsComponent();
    }

    void LuaComponent::bindAnimationComponent() {
        luabind::module(mState) [
                luabind::class_<AnimationComponent>("AnimationComponent")
                // TODO
                ];
    }

    void LuaComponent::bindAudioComponent() {
        luabind::module(mState) [
                luabind::class_<AudioComponent>("AudioComponent")
                // TODO
                ];
    }

    void LuaComponent::bindGraphicsComponent() {
        luabind::module(mState) [
                luabind::class_<GraphicsComponent>("GraphicsComponent")
                .def("getWindowWidth",&GraphicsComponent::getWindowWidth)
                .def("getWindowHeight",&GraphicsComponent::getWindowHeight)
                .def("setWindowShouldClose",&GraphicsComponent::setWindowShouldClose)
                ];
    }

    void LuaComponent::bindPhysicsComponent() {
        luabind::module(mState) [
                luabind::class_<PhysicsComponent>("PhysicsComponent")
                .def("setDebug",&PhysicsComponent::setDebug)
                ];
    }

    void LuaComponent::bindProject() {
        luabind::module(mState) [
                luabind::class_<Project>("Project")
                ];
    }

    void LuaComponent::bindScene() {
        luabind::module(mState) [
                luabind::class_<Scene>("Scene")
                .def("getCameraMovementSpeed",&Scene::getCameraMovementSpeed)
                .def("setCameraMovementSpeed",&Scene::setCameraMovementSpeed)
                .def("getSceneObjectByUuid",&Scene::getSceneObjectByUuid)
                .def("getSceneObjectByName",&Scene::getSceneObjectByName)
                ];
    }

    void LuaComponent::bindSceneObject() {
        luabind::module(mState) [
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
                .def("addChild",&SceneObject::addChild,luabind::adopt(_2))

                .def("copyTransform",&SceneObject::copyTransform)

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

    void LuaComponent::bindTransform3D() {
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

    void LuaComponent::bindTime() {
        luabind::module(mState) [
                luabind::class_<Time>("Time")
                .def("getCurrentTime",&Time::getCurrentTime)
                .def("getLastTime",&Time::getLastTime)
                .def("getTimeDelta",&Time::getTimeDelta)
                ];
    }

    void LuaComponent::bindSDL() {
        bindSDL_Event();
    }

    void LuaComponent::bindSDL_Event() {
        luabind::module(mState)
                [
                // SDL_Event =======================================================
                luabind::class_<SDL_Event>("Event")
                .def_readonly("type", &SDL_Event::type)
                // Keyboard
                .def_readonly("key", &SDL_Event::key)
                // Controller
                .def_readonly("cbutton", &SDL_Event::cbutton)
                .def_readonly("caxis",   &SDL_Event::caxis)
                // Joystick
                .def_readonly("jbutton", &SDL_Event::jbutton)
                .def_readonly("jhat",    &SDL_Event::jhat)
                .def_readonly("jaxis",   &SDL_Event::jaxis)
                // Mouse
                .def_readonly("motion", &SDL_Event::motion)
                .def_readonly("button", &SDL_Event::button)
                .def_readonly("wheel",  &SDL_Event::wheel)
                // SDL_EventType ===============================================
                .enum_("EventType")
                [
                // Keys
                luabind::value("KEYUP",SDL_EventType::SDL_KEYUP),
                luabind::value("KEYDOWN",SDL_EventType::SDL_KEYDOWN),
                // Joystick
                luabind::value("JOY_BUTTONDOWN",SDL_JOYBUTTONDOWN),
                luabind::value("JOY_BUTTONUP",SDL_JOYBUTTONUP),
                luabind::value("JOY_HATMOTION",SDL_JOYHATMOTION),
                luabind::value("JOY_AXISMOTION",SDL_JOYAXISMOTION),
                // Controller
                luabind::value("CONTROLLER_BUTTONDOWN",SDL_CONTROLLERBUTTONDOWN),
                luabind::value("CONTROLLER_BUTTONUP",SDL_CONTROLLERBUTTONUP),
                luabind::value("CONTROLLER_AXISMOTION",SDL_CONTROLLERAXISMOTION),
                // Mouse
                luabind::value("MOUSE_MOTION",SDL_MOUSEMOTION),
                luabind::value("MOUSE_BUTTONUP",SDL_MOUSEBUTTONUP),
                luabind::value("MOUSE_BUTTONDOWN",SDL_MOUSEBUTTONDOWN)
                ]
                // SDLK_* Definitions ==========================================
                .enum_("Key")
                [
                luabind::value("KEY_RETURN",SDLK_RETURN),
                luabind::value("KEY_SPACE",SDLK_SPACE),
                luabind::value("KEY_LEFT",SDLK_LEFT),
                luabind::value("KEY_RIGHT",SDLK_RIGHT),
                luabind::value("KEY_UP",SDLK_UP),
                luabind::value("KEY_DOWN",SDLK_DOWN),
                // Alpha
                luabind::value("KEY_a",SDLK_a),
                luabind::value("KEY_b",SDLK_a),
                luabind::value("KEY_c",SDLK_c),
                luabind::value("KEY_d",SDLK_d),
                luabind::value("KEY_e",SDLK_e),
                luabind::value("KEY_f",SDLK_f),
                luabind::value("KEY_g",SDLK_g),
                luabind::value("KEY_h",SDLK_h),
                luabind::value("KEY_i",SDLK_i),
                luabind::value("KEY_j",SDLK_j),
                luabind::value("KEY_k",SDLK_k),
                luabind::value("KEY_l",SDLK_l),
                luabind::value("KEY_m",SDLK_m),
                luabind::value("KEY_n",SDLK_n),
                luabind::value("KEY_o",SDLK_o),
                luabind::value("KEY_p",SDLK_p),
                luabind::value("KEY_q",SDLK_q),
                luabind::value("KEY_r",SDLK_r),
                luabind::value("KEY_s",SDLK_s),
                luabind::value("KEY_t",SDLK_t),
                luabind::value("KEY_u",SDLK_u),
                luabind::value("KEY_v",SDLK_v),
                luabind::value("KEY_w",SDLK_w),
                luabind::value("KEY_x",SDLK_x),
                luabind::value("KEY_y",SDLK_y),
                luabind::value("KEY_z",SDLK_z),
                // Numero
                luabind::value("KEY_0",SDLK_0),
                luabind::value("KEY_1",SDLK_1),
                luabind::value("KEY_2",SDLK_2),
                luabind::value("KEY_3",SDLK_3),
                luabind::value("KEY_4",SDLK_4),
                luabind::value("KEY_5",SDLK_5),
                luabind::value("KEY_6",SDLK_6),
                luabind::value("KEY_7",SDLK_7),
                luabind::value("KEY_8",SDLK_8),
                luabind::value("KEY_9",SDLK_9)
                ],

                // SDL_ControllerButtonEvent =======================================
                luabind::class_<SDL_ControllerButtonEvent>("ControllerButtonEvent")
                .def_readonly("type",&SDL_ControllerButtonEvent::type)
                .def_readonly("timestamp",&SDL_ControllerButtonEvent::timestamp)
                .def_readonly("which",&SDL_ControllerButtonEvent::which)
                .def_readonly("button",&SDL_ControllerButtonEvent::button)
                .def_readonly("state",&SDL_ControllerButtonEvent::state)
                .enum_("GameControllerButton")
                [
                luabind::value("CBTN_INVALID",SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID),
                // Face
                luabind::value("C_BTN_A", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A),
                luabind::value("C_BTN_B", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B),
                luabind::value("C_BTN_X", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X),
                luabind::value("C_BTN_Y", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y),
                // Face.2
                luabind::value("C_BTN_BACK",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK),
                luabind::value("C_BTN_GUIDE", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_GUIDE),
                luabind::value("C_BTN_START", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START),
                // Stick
                luabind::value("C_BTN_LEFT_STICK",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK),
                luabind::value("C_BTN_RIGHT_STICK", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK),
                // Shoulder
                luabind::value("C_BTN_LEFT_SHOULDER",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
                luabind::value("C_BTN_RIGHT_SHOULDER", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),
                // DPAD
                luabind::value("C_BTN_DPAD_UP",    SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP),
                luabind::value("C_BTN_DPAD_DOWN",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN),
                luabind::value("C_BTN_DPAD_LEFT",  SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT),
                luabind::value("C_BTN_DPAD_RIGHT", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                ],

                // SDL_ControllerAxisEvent =========================================
                luabind::class_<SDL_ControllerAxisEvent>("ControllerAxisEvent")
                .def_readonly("type",&SDL_ControllerAxisEvent::type)
                .def_readonly("timestamp",&SDL_ControllerAxisEvent::timestamp)
                .def_readonly("axis",&SDL_ControllerAxisEvent::axis)
                .def_readonly("value",&SDL_ControllerAxisEvent::value)
                .enum_("GameControllerAxis")
                [
                luabind::value("C_AXIS_INVALID",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_INVALID),
                // Left
                luabind::value("C_AXIS_LEFT_X",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX),
                luabind::value("C_AXIS_LEFT_Y",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY),
                // Right
                luabind::value("C_AXIS_RIGHT_X",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX),
                luabind::value("C_AXIS_RIGHT_Y",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY),
                // Triggers
                luabind::value("C_AXIS_TRIGGER_LEFT",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT),
                luabind::value("C_AXIS_TRIGGER_RIGHT",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT),
                // Max
                luabind::value("C_AXIS_MAX",SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_MAX)
                ],

                // SDL_KeyboardEvent ===============================================
                luabind::class_<SDL_KeyboardEvent>("KeyboardEvent")
                .def_readonly("keysym",&SDL_KeyboardEvent::keysym),

                // SDL_Keysym ======================================================
                luabind::class_<SDL_Keysym>("Keysym")
                .def_readonly("sym",&SDL_Keysym::sym),

                // SDL_Keycode =====================================================
                luabind::class_<SDL_Keycode>("Keycode"),

                // SDL_JoyButtonEvent ==============================================
                luabind::class_<SDL_JoyButtonEvent>("JoyButtonEvent")
                .def_readonly("type",&SDL_JoyButtonEvent::type)
                .def_readonly("timestamp",&SDL_JoyButtonEvent::timestamp)
                .def_readonly("which",&SDL_JoyButtonEvent::which)
                .def_readonly("button",&SDL_JoyButtonEvent::button)
                .def_readonly("state",&SDL_JoyButtonEvent::state),

                // SDL_JoyHatEvent =================================================
                luabind::class_<SDL_JoyHatEvent>("JoyHatEvent")
                .def_readonly("type",&SDL_JoyHatEvent::type)
                .def_readonly("timestamp",&SDL_JoyHatEvent::timestamp)
                .def_readonly("which",&SDL_JoyHatEvent::which)
                .def_readonly("hat",&SDL_JoyHatEvent::hat)
                .def_readonly("value",&SDL_JoyHatEvent::value),

                // SDL_JoyAxisEvent ================================================
                luabind::class_<SDL_JoyAxisEvent>("JoyAxisEvent")
                .def_readonly("type",&SDL_JoyAxisEvent::type)
                .def_readonly("timestamp",&SDL_JoyAxisEvent::timestamp)
                .def_readonly("which",&SDL_JoyAxisEvent::which)
                .def_readonly("axis",&SDL_JoyAxisEvent::axis)
                .def_readonly("value",&SDL_JoyAxisEvent::value),

                // SDL_MouseMotionEvent ============================================
                luabind::class_<SDL_MouseMotionEvent>("MouseMotionEvent")
                .def_readonly("type",&SDL_MouseMotionEvent::type)
                .def_readonly("timestamp",&SDL_MouseMotionEvent::timestamp)
                .def_readonly("windowID",&SDL_MouseMotionEvent::windowID)
                .def_readonly("which",&SDL_MouseMotionEvent::which)
                .def_readonly("state",&SDL_MouseMotionEvent::state)
                .def_readonly("x",&SDL_MouseMotionEvent::x)
                .def_readonly("y",&SDL_MouseMotionEvent::y)
                .def_readonly("xrel",&SDL_MouseMotionEvent::xrel)
                .def_readonly("yrel",&SDL_MouseMotionEvent::yrel)

                ]; // end luabind::module
    }

    void LuaComponent::bindAssetClasses() {
        bindAudioInstance();
        bindFontInstance();
        //bindAnimationInstance();
        //bindAssimpModelInstance();
        //bindLightInstance();
        bindShaderInstance();
        //bindSpriteInstance();
        bindPhysicsObjectInstance();
    }

    void LuaComponent::bindFontInstance() {
        luabind::module(mState) [
                luabind::class_<FontInstance>("FontInstance")
                .def("setText",&FontInstance::setText)
                .def("getText",&FontInstance::getText)
                ];
    }

    void LuaComponent::bindAudioInstance() {
        luabind::module(mState) [
                luabind::class_<AudioInstance>("AudioInstance")
                .def("play",&AudioInstance::play)
                .def("pause",&AudioInstance::pause)
                .def("stop",&AudioInstance::stop)
                .def("getStatus",&AudioInstance::getStatus),

                luabind::class_<AudioStatus>("AudioStatus")
                .enum_("AudioStatus") [
                luabind::value("PLAYING", AudioStatus::PLAYING),
                luabind::value("PAUSED",  AudioStatus::PAUSED),
                luabind::value("STOPPED", AudioStatus::STOPPED)
                ]
                ];
    }

    void LuaComponent::bindShaderInstance() {
        luabind::module(mState) [
                luabind::class_<ShaderInstance>("ShaderInstance")
                .def("getUuid", &ShaderInstance::getUuid     )

                .def("setUniform1f", &ShaderInstance::setUniform1f )
                ];
    }

    void LuaComponent::bindPhysicsObjectInstance() {
        luabind::module(mState) [
                luabind::class_<PhysicsObjectInstance>("PhysicsObjectInstance")
                .def("getUuid", &PhysicsObjectInstance::getUuid     )
                .def("setLinearVelocity", &PhysicsObjectInstance::setLinearVelocity)
                ];
    }

    void LuaComponent::setLuaScriptMap(map<SceneObject*,LuaScriptInstance*>* scriptMap) {
        mScriptMap = scriptMap;
    }

    void LuaComponent::stackDump() {
        cerr << "LuaComponent: Stack Dump!" << endl;
        int i;
        int top = lua_gettop(mState);
        // repeat for each level
        for (i = 1; i <= top; i++) {
            int t = lua_type(mState, i);
            switch (t) {
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

    bool LuaComponent::update() {
        if (VERBOSE) {
            cout << "LuaComponent: Update Called" << endl;
        }
        map<SceneObject*,LuaScriptInstance*>::iterator scriptIt;
        for (scriptIt=mScriptMap->begin(); scriptIt != mScriptMap->end(); scriptIt++) {
            SceneObject* key = scriptIt->first;
            if (key->getDeleteFlag()) {
                if (DEBUG) {
                    cout << "LuaComponent: Skipping update on " << key->getUuid() << endl;
                }
                continue;
            }
            LuaScriptInstance *value = scriptIt->second;
            if (!executeScriptUpdate(key)) {
                return false;
            }
            if (key->hasFocus()) {
                if (!executeScriptKeyHandler(key)) {
                    return false;
                }
            }
            if (key->hasEvents()) {
                if (!executeScriptEventHandler(key)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool LuaComponent::executeScriptUpdate(SceneObject* sceneObject) {
        string id = sceneObject->getUuid();
        if (VERBOSE) {
            cout << "LuaComponent: Calling onUpdate for " << sceneObject->getNameUuidString() << endl;
        }
        try {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_UPDATE_FUNCTION];
            luabind::call_function<void>(funq,sceneObject);
        } catch (luabind::error &e) {
            string error = lua_tostring( e.state(), -1 );
            cerr << "LuaComponent: onUpdate exception:" << endl << e.what() << endl;
            cerr << error << endl;

            return false;
        }
        return true;
    }

    bool LuaComponent::executeScriptInit(SceneObject* sceneObject) {
        string id = sceneObject->getUuid();
        if (VERBOSE) {
            cout << "LuaComponent: Calling onInit for " << sceneObject->getNameUuidString() << endl << flush;
        }
        try {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_INIT_FUNCTION];
            luabind::call_function<void>(funq,sceneObject);
        } catch (luabind::error &e) {
            cerr << "LuaComponent: onInit exception:" << endl << e.what() << endl;
            return false;
        }
        return true;
    }

    bool LuaComponent::executeScriptKeyHandler(SceneObject* sceneObject) {
        string id = sceneObject->getUuid();
        if (VERBOSE) {
            cout << "LuaComponent: Calling onInput for " << sceneObject->getNameUuidString() << endl << flush;
        }
        try {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_INPUT_FUNCTION];
            vector<SDL_Event>::iterator it;
            for (it=mSDLEvents.begin();it!=mSDLEvents.end();it++) {
                SDL_Event next = (*it);
                luabind::call_function<void>(funq,sceneObject,next);
            }
        } catch (luabind::error &e) {
            cerr << "LuaComponent: onInput exception:" << endl << e.what() << endl;
            return false;
        }
        return true;
    }

    bool LuaComponent::executeScriptEventHandler(SceneObject* sceneObject) {
        string id = sceneObject->getUuid();
        if (VERBOSE) {
            cout << "LuaComponent: Calling onEvent for " << sceneObject->getNameUuidString() << endl << flush;
        }
        try {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_EVENT_FUNCTION];
            vector<Event*>* events = sceneObject->getEventQueue();
            vector<Event*>::iterator eventIt;
            for (eventIt=events->begin();eventIt!=events->end(); eventIt++) {
                luabind::call_function<void>(funq,sceneObject,(*eventIt));
            }
            sceneObject->cleanupEvents();
        } catch (luabind::error &e) {
            cerr << "LuaComponent: onEvent exception:" << endl << e.what() << endl;
            return false;
        }
        return true;
    }

    void LuaComponent::setSDL_Events(vector<SDL_Event> events) {
        mSDLEvents = events;
    }

    void LuaComponent::bindDreamEvent() {
        luabind::module(mState) [
                luabind::class_<Event>("DreamEvent")
                .def("getSender",&Event::getSender)
                .def("getType",&Event::getType)
                .def("getAttribute",&Event::getAttribute)
                .def("setAttribute",&Event::setAttribute)
                ];
    }

    void LuaComponent::bindCamera() {
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

    void LuaComponent::bindGameController() {
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

} // End of Dream

using namespace std;

int errorHandler(lua_State *L) {
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
