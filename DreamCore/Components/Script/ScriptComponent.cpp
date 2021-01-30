/*
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

#include "ScriptComponent.h"
#include "ScriptRuntime.h"
#include "Project/Project.h"
#include "Components/Event.h"
#include "Components/Transform.h"
#include "Components/Time.h"
#include "Components/Path/PathRuntime.h"
#include "Components/Animation/AnimationRuntime.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioRuntime.h"
#include "Components/Audio/AudioStatus.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/Camera.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Light/LightRuntime.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Input/InputComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Physics/PhysicsObjectRuntime.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Components/Window/WindowComponent.h"
#include "Project/ProjectRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"

#define SOL_CHECK_ARGUMENTS 1
#include <sol.h>
#include <glm/gtc/matrix_transform.hpp>

using std::stringstream;
using std::exception;
using std::cout;
using std::cerr;
using std::string;

static int l_my_print(lua_State* L)
{
    int nargs = lua_gettop(L);
    stringstream stream;
    for (int i=1; i <= nargs; ++i)
    {
        stream << lua_tostring(L, i);
    }

    string out = stream.str();
    for (auto listener : octronic::dream::ScriptComponent::PrintListeners)
    {
        listener->onPrint(out);
    }
    return 0;
}

static const struct luaL_Reg printlib [] = {{"print", l_my_print}, {nullptr, nullptr}};

namespace octronic::dream
{
    ScriptComponent::ScriptComponent
    (ProjectRuntime* runtime, ScriptCache* cache)
        : Component(runtime),
          mScriptCache(cache)
    {
        LOG_TRACE( "Constructing Object" );
    }

    ScriptComponent::~ScriptComponent
    ()
    {
        LOG_TRACE("Destroying Object");
        lock();
        if (State != nullptr)
        {
            lua_close(State);
            State = nullptr;
        }
        unlock();
    }

    bool
    ScriptComponent::init
    ()
    {
        LOG_DEBUG( "Initialising LuaComponent" );
        State = luaL_newstate();
        sol::state_view sView(State);
        sView.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string);
        sol::table comps(State, sol::create);
        sView[COMPONENTS_TBL] = comps;

        // Register print callback

        lua_getglobal(State, "_G");
        luaL_setfuncs(State, printlib, 0);
        lua_pop(State, 1);

        LOG_DEBUG( "Got a sol state" );
        exposeAPI();
        return true;
    }

    // API Exposure Methods ======================================================



    void
    ScriptComponent::exposeProjectRuntime
    ()
    {
        debugRegisteringClass("ProjectRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<ProjectRuntime>(
                    "ProjectRuntime",
                    "getTime",&ProjectRuntime::getTime,
                    "getAssetDefinition",&ProjectRuntime::getAssetDefinitionByUuid,
                    "getEntity",&ProjectRuntime::getEntityRuntimeByUuid,
                    "windowWidth",&ProjectRuntime::getWindowWidth,
                    "windowHeight",&ProjectRuntime::getWindowHeight
                    );

        stateView["Runtime"] = mProjectRuntime;
    }

    void
    ScriptComponent::exposeProjectDirectory
    ()
    {
        debugRegisteringClass("ProjectDirectory");
        sol::state_view stateView(State);
        stateView.new_usertype<ProjectDirectory>(
                    "ProjectDirectory",
                    "getAssetPath",static_cast<string (ProjectDirectory::*)(UuidType) const>(&ProjectDirectory::getAssetAbsolutePath)
                    );

        stateView["Directory"] = mProjectRuntime->getProject()->getDirectory();
    }

    void
    ScriptComponent::exposeCamera
    ()
    {
        debugRegisteringClass("Camera");
        sol::state_view stateView(State);
        stateView.new_usertype<Camera>(
                    "Camera",
                    "flyForward",&Camera::flyForward,
                    "flyBackward",&Camera::flyBackward,
                    "flyLeft",&Camera::flyLeft,
                    "flyRight",&Camera::flyRight,
                    "flyUp",&Camera::flyUp,
                    "flyDown",&Camera::flyDown,
                    "setTranslation",static_cast<void(Camera::*)(float,float,float)>(&Camera::setTranslation),
                    "setTranslation",static_cast<void(Camera::*)(const Vector3&)>(&Camera::setTranslation),
                    "getTranslation",&Camera::getTranslation,
                    "getFocusedEntity",&Camera::getFocusedEntity,
                    "getFocusedObjectTheta",&Camera::getFocusedObjectTheta
                    );

        stateView.new_enum(
                    "CameraMovement",
                    "FORWARD",  Constants::CAMERA_MOVEMENT_FORWARD,
                    "BACKWARD", Constants::CAMERA_MOVEMENT_BACKWARD,
                    "LEFT",     Constants::CAMERA_MOVEMENT_LEFT,
                    "RIGHT",    Constants::CAMERA_MOVEMENT_RIGHT
                    );

        stateView.new_enum(
                    "FrustumPlane",
                    "Back",Frustum::Plane::PLANE_BACK,
                    "Front",Frustum::Plane::PLANE_FRONT,
                    "Right",Frustum::Plane::PLANE_RIGHT,
                    "Left",Frustum::Plane::PLANE_LEFT,
                    "Top",Frustum::Plane::PLANE_TOP,
                    "Bottom",Frustum::Plane::PLANE_BOTTOM
                    );

        stateView.new_enum(
                    "FrustumTestResult",
                    "Inside",Frustum::TestResult::TEST_INSIDE,
                    "Outside",Frustum::TestResult::TEST_OUTSIDE,
                    "Intersect",Frustum::TestResult::TEST_INTERSECT
                    );
    }

    void
    ScriptComponent::exposePathRuntime
    ()
    {
        debugRegisteringClass("PathRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<PathRuntime>(
                    "PathRuntime",
                    "generate",&PathRuntime::generate,
                    "getSplinePoints",&PathRuntime::getSplinePoints,
                    "getSplinePoint",&PathRuntime::getSplinePoint,
                    "getUStep",&PathRuntime::getUStep,
                    "setUStep",&PathRuntime::setUStep,
                    "stepPath",&PathRuntime::stepPath
                    );
    }

    void
    ScriptComponent::exposeGraphicsComponent
    ()
    {
        debugRegisteringClass("GraphicsComponent");
        sol::state_view stateView(State);
        stateView.new_usertype<GraphicsComponent>("GraphicsComponent");
        stateView[COMPONENTS_TBL]["Graphics"] = mProjectRuntime->getGraphicsComponent();
    }

    void
    ScriptComponent::exposeLightRuntime
    ()
    {
        debugRegisteringClass("LightRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<LightRuntime>("LightRuntime");
    }

    void
    ScriptComponent::exposeShaderRuntime
    ()
    {
        debugRegisteringClass("ShaderRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<ShaderRuntime>(
                    "ShaderRuntime",
                    "getUuid", &ShaderRuntime::getUuid,
                    "addUniform",&ShaderRuntime::addUniform
                    );

        debugRegisteringClass("ShaderUniform");

        stateView.new_usertype<ShaderUniform>("ShaderUniform");
        stateView.new_enum(
                    "UniformType",
                    "INT1",UniformType::INT1,
                    "INT2",UniformType::INT2,
                    "INT3",UniformType::INT3,
                    "INT4",UniformType::INT4,
                    "UINT1",UniformType::UINT1,
                    "UINT2",UniformType::UINT2,
                    "UINT3",UniformType::UINT3,
                    "UINT4",UniformType::UINT4,
                    "FLOAT1",UniformType::FLOAT1,
                    "FLOAT2",UniformType::FLOAT2,
                    "FLOAT3",UniformType::FLOAT3,
                    "FLOAT4",UniformType::FLOAT4
                    );
    }

    void
    ScriptComponent::exposePhysicsComponent
    ()
    {
        debugRegisteringClass("PhysicsComponent");
        sol::state_view stateView(State);
        stateView.new_usertype<PhysicsComponent>("PhysicsComponent",
                                                 "setDebug",&PhysicsComponent::setDebug
                                                 );

        stateView[COMPONENTS_TBL]["Physics"] = mProjectRuntime->getPhysicsComponent();
    }

    void
    ScriptComponent::exposePhysicsObjectRuntime
    ()
    {
        debugRegisteringClass("PhysicsObjectRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<PhysicsObjectRuntime>(
                    "PhysicsObjectRuntime",
                    "getUuid", &PhysicsObjectRuntime::getUuid,
                    "getMass", &PhysicsObjectRuntime::getMass,
                    "setMass", &PhysicsObjectRuntime::setMass,
                    "getLinearVelocity", &PhysicsObjectRuntime::getLinearVelocity,
                    "setLinearVelocity", &PhysicsObjectRuntime::setLinearVelocity,
                    "setLinearFactor", &PhysicsObjectRuntime::setLinearFactor,
                    "setAngularFactor", &PhysicsObjectRuntime::setAngularFactor,
                    "setAngularVelocity", &PhysicsObjectRuntime::setAngularVelocity,
                    "getRestitution", &PhysicsObjectRuntime::getRestitution,
                    "setRestitution", &PhysicsObjectRuntime::setRestitution,
                    "getFriction", &PhysicsObjectRuntime::getFriction,
                    "setFriction", &PhysicsObjectRuntime::setFriction,
                    "clearForces",&PhysicsObjectRuntime::clearForces,
                    "getCenterOfMassPosition",&PhysicsObjectRuntime::getCenterOfMassPosition,
                    "setCenterOfMassTransformTx",&PhysicsObjectRuntime::setCenterOfMassTransformTx,
                    "setCenterOfMassTransform3f",&PhysicsObjectRuntime::setCenterOfMassTransform3f,
                    "setCenterOfMassTransform3fv",&PhysicsObjectRuntime::setCenterOfMassTransform3fv,
                    "setCenterOfMassTransformMat4",&PhysicsObjectRuntime::setCenterOfMassTransformMat4,
                    "setKinematicObject",&PhysicsObjectRuntime::setKinematic,
                    "setGameModeCharacter",&PhysicsObjectRuntime::setCameraControllableCharacter
                    );
    }

    void
    ScriptComponent::exposeEntityRuntime
    ()
    {
        debugRegisteringClass("EntityRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<EntityRuntime>(
                    "EntityRuntime",
                    "getName",&EntityRuntime::getName,
                    "getNameAndUuidString",&EntityRuntime::getNameAndUuidString,
                    "getScene",&EntityRuntime::getSceneRuntime,
                    "getChildByUuid",&EntityRuntime::getChildRuntimeByUuid,
                    "getParent",&EntityRuntime::getParentRuntime,
                    "setParent",&EntityRuntime::setParentRuntime,
                    "getTransform",&EntityRuntime::getTransform,
                    "getPathRuntime",&EntityRuntime::getPathRuntime,
                    "getAnimationRuntime",&EntityRuntime::getAnimationRuntime,
                    "getAudioRuntime",&EntityRuntime::getAudioRuntime,
                    "getModelRuntime",&EntityRuntime::getModelRuntime,
                    "getLightRuntime",&EntityRuntime::getLightRuntime,
                    "getPhysicsObjectRuntime",&EntityRuntime::getPhysicsObjectRuntime,
                    "hasPathRuntime",&EntityRuntime::hasPathRuntime,
                    "hasAudioRuntime",&EntityRuntime::hasAudioRuntime,
                    "hasModelRuntime",&EntityRuntime::hasModelRuntime,
                    "hasLightRuntime",&EntityRuntime::hasLightRuntime,
                    "hasPhysicsObjectRuntime",&EntityRuntime::hasPhysicsObjectRuntime,
                    "getDeleted",&EntityRuntime::getDeleted,
                    "setDeleted",&EntityRuntime::setDeleted,
                    "getHidden",&EntityRuntime::getHidden,
                    "setHidden",&EntityRuntime::setHidden,
                    "addEvent",&EntityRuntime::addEvent,
                    "replaceAssetUuid",&EntityRuntime::replaceAssetUuid,
                    "translateWithChildren",&EntityRuntime::translateWithChildren,
                    "preTranslateWithChildren",&EntityRuntime::preTranslateWithChildren,
                    "transformOffsetInitial",&EntityRuntime::transformOffsetInitial,
                    "translateOffsetInitial",&EntityRuntime::translateOffsetInitial,
                    "translateOffsetInitialWithChildren",&EntityRuntime::translateOffsetInitialWithChildren,
                    "containedInFrustum",&EntityRuntime::containedInFrustum,
                    "containedInFrustumAfterTransform",&EntityRuntime::containedInFrustum,
                    "exceedsFrustumPlaneAtTranslation",&EntityRuntime::exceedsFrustumPlaneAtTranslation,
                    "addChildFromTemplateUuid",&EntityRuntime::addChildFromTemplateUuid,
                    "getObjectLifetime",&EntityRuntime::getObjectLifetime,
                    "getDieAfter",&EntityRuntime::getDieAfter
                    );
    }

    void
    ScriptComponent::exposeTransform
    ()
    {
        debugRegisteringClass("Transform");
        sol::state_view stateView(State);
        stateView.new_usertype<Transform>(
                    "Transform",
                    "getMatrix",&Transform::getMatrix,
                    "setMatrix",&Transform::setMatrix,
                    "decomposeMatrix",&Transform::decomposeMatrix,
                    "recomposeMatrix",&Transform::recomposeMatrix,
                    "translate",&Transform::translate,
                    "translate3f",&Transform::translate3f,
                    "preTranslate",&Transform::preTranslate,
                    "getTranslation",&Transform::getTranslation
                    );

        stateView.new_usertype<MatrixDecomposition>(
                    "MatrixDecomposition",
                    "translation",&MatrixDecomposition::translation,
                    "rotation",&MatrixDecomposition::rotation,
                    "scale",&MatrixDecomposition::scale,
                    "skew",&MatrixDecomposition::skew,
                    "perspective",&MatrixDecomposition::perspective
                    );
    }

    void
    ScriptComponent::exposeTime
    ()
    {
        debugRegisteringClass("Time");
        sol::state_view stateView(State);
        stateView.new_usertype<Time>(
                    "Time",
                    "getCurrentFrameTime",&Time::getCurrentFrameTime,
                    "getLastFrameTime",&Time::getLastFrameTime,
                    "getFrameTimeDelta",&Time::getFrameTimeDelta,
                    "perSecond",&Time::perSecond
                    );

        stateView["Time"] = mProjectRuntime->getTime();
    }

    void
    ScriptComponent::exposeModelRuntime
    ()
    {
        debugRegisteringClass("ModelRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<ModelRuntime>("ModelRuntime");
    }

    void
    ScriptComponent::exposeEvent
    ()
    {
        debugRegisteringClass("Event");
        sol::state_view stateView(State);
        stateView.new_usertype<Event>(
                    "Event",
                    "getAttribute",&Event::getAttribute,
                    "setAttribute",&Event::setAttribute
                    );

        /*
        stateView.new_usertype<CollisionData>(
            "CollisionData",
            "present",&CollisionData::present,
            "impulse",&CollisionData::impulse,
            "position",&CollisionData::position
        );
        */
    }

    void
    ScriptComponent::exposeWindowComponent
    ()
    {
        debugRegisteringClass("WindowComponent");
        sol::state_view stateView(State);
        stateView.new_usertype<WindowComponent>(
                    "WindowComponent",
                    "getWidth",&WindowComponent::getWidth,
                    "getHeight",&WindowComponent::getHeight
                    );
        stateView[COMPONENTS_TBL]["Window"] = mProjectRuntime->getWindowComponent();
    }

    void
    ScriptComponent::exposeInputComponent
    ()
    {
        debugRegisteringClass("InputComponent");
        sol::state_view stateView(State);
        stateView.new_usertype<InputComponent>(
                    "InputComponent",
                    "isKeyDown",&InputComponent::isKeyDown,
                    "getKeyboardState",&InputComponent::getKeyboardState,
                    "getMouseState",&InputComponent::getMouseState,
                    "getJoystickState",&InputComponent::getJoystickState,
                    "getJoystickMapping",&InputComponent::getJoystickMapping,
                    "getJoystickNavigation", &InputComponent::getJoystickNavigation,
                    "getJoystickCount", &InputComponent::getJoystickCount
                    );

        stateView.new_usertype<JoystickMapping>
                (
                    "JoystickMapping",
                    "AnalogLeftXAxis",&JoystickMapping::AnalogLeftXAxis ,
                    "AnalogLeftYAxis",&JoystickMapping::AnalogLeftYAxis ,
                    "AnalogRightXAxis",&JoystickMapping::AnalogRightXAxis ,
                    "AnalogRightYAxis",&JoystickMapping::AnalogRightYAxis ,
                    "FaceButtonNorth",&JoystickMapping::FaceButtonNorth ,
                    "FaceButtonEast",&JoystickMapping::FaceButtonEast ,
                    "FaceButtonSouth",&JoystickMapping::FaceButtonSouth ,
                    "FaceButtonWest",&JoystickMapping::FaceButtonWest ,
                    "FaceButtonSelect",&JoystickMapping::FaceButtonSelect ,
                    "FaceButtonStart",&JoystickMapping::FaceButtonStart ,
                    "FaceButtonHome",&JoystickMapping::FaceButtonHome ,
                    "ShoulderLeft",&JoystickMapping::ShoulderLeft ,
                    "ShoulderRight",&JoystickMapping::ShoulderRight ,
                    "TriggerLeftButton",&JoystickMapping::TriggerLeftButton ,
                    "TriggerRightButton",&JoystickMapping::TriggerRightButton ,
                    "AnalogLeftButton",&JoystickMapping::AnalogLeftButton ,
                    "AnalogRightButton",&JoystickMapping::AnalogRightButton ,
                    "DPadNorth",&JoystickMapping::DPadNorth,
                    "DPadWest",&JoystickMapping::DPadWest,
                    "DPadSouth",&JoystickMapping::DPadSouth,
                    "DPadEast",&JoystickMapping::DPadEast
                    );

        stateView.new_usertype<JoystickState>(
                    "JoystickState",
                    "getButtonData", &JoystickState::getButtonData,
                    "setButtonData", &JoystickState::setButtonData,
                    "getAxisData", &JoystickState::getAxisData,
                    "setAxisData", &JoystickState::setAxisData,
                    "getName", &JoystickState::getName,
                    "getButtonCount", &JoystickState::getButtonCount,
                    "setButtonCount", &JoystickState::setButtonCount,
                    "getAxisCount", &JoystickState::getAxisCount,
                    "setAxisCount", &JoystickState::setAxisCount,
                    "getDeadZone", &JoystickState::getDeadZone
                    );

        stateView.new_usertype<JoystickNavigation>(
                    "JoystickNavigation",
                    "update", &JoystickNavigation::update,
                    "setHeading", &JoystickNavigation::setHeading,
                    "getHeading", &JoystickNavigation::getHeading
                    );

        stateView.new_enum(
                    "KeyboardMapping",
                    "KEY_UNKNOWN",  -1,
                    "KEY_SPACE",  32,
                    "KEY_APOSTROPHE",  39,  /* ' */
                    "KEY_COMMA",  44,  /* , */
                    "KEY_MINUS",  45,  /* - */
                    "KEY_PERIOD",  46,  /* . */
                    "KEY_SLASH",  47,  /* / */
                    "KEY_0",  48,
                    "KEY_1",  49,
                    "KEY_2",  50,
                    "KEY_3",  51,
                    "KEY_4",  52,
                    "KEY_5",  53,
                    "KEY_6",  54,
                    "KEY_7",  55,
                    "KEY_8",  56,
                    "KEY_9",  57,
                    "KEY_SEMICOLON",  59,  /* , */
                    "KEY_EQUAL",  61,  /* ", */
                    "KEY_A",  65,
                    "KEY_B",  66,
                    "KEY_C",  67,
                    "KEY_D",  68,
                    "KEY_E",  69,
                    "KEY_F",  70,
                    "KEY_G",  71,
                    "KEY_H",  72,
                    "KEY_I",  73,
                    "KEY_J",  74,
                    "KEY_K",  75,
                    "KEY_L",  76,
                    "KEY_M",  77,
                    "KEY_N",  78,
                    "KEY_O",  79,
                    "KEY_P",  80,
                    "KEY_Q",  81,
                    "KEY_R",  82,
                    "KEY_S",  83,
                    "KEY_T",  84,
                    "KEY_U",  85,
                    "KEY_V",  86,
                    "KEY_W",  87,
                    "KEY_X",  88,
                    "KEY_Y",  89,
                    "KEY_Z",  90,
                    "KEY_LEFT_BRACKET",  91,  /* [ */
                    "KEY_BACKSLASH",  92,  /* \ */
                    "KEY_RIGHT_BRACKET",  93,  /* ] */
                    "KEY_GRAVE_ACCENT",  96,  /* ` */
                    "KEY_WORLD_1",  161, /* non-US #1 */
                    "KEY_WORLD_2",  162, /* non-US #2 */
                    "KEY_ESCAPE",  256,
                    "KEY_ENTER",  257,
                    "KEY_TAB",  258,
                    "KEY_BACKSPACE",  259,
                    "KEY_INSERT",  260,
                    "KEY_DELETE",  261,
                    "KEY_RIGHT",  262,
                    "KEY_LEFT",  263,
                    "KEY_DOWN",  264,
                    "KEY_UP",  265,
                    "KEY_PAGE_UP",  266,
                    "KEY_PAGE_DOWN",  267,
                    "KEY_HOME",  268,
                    "KEY_END",  269,
                    "KEY_CAPS_LOCK",  280,
                    "KEY_SCROLL_LOCK",  281,
                    "KEY_NUM_LOCK",  282,
                    "KEY_PRINT_SCREEN",  283,
                    "KEY_PAUSE",  284,
                    "KEY_F1",  290,
                    "KEY_F2",  291,
                    "KEY_F3",  292,
                    "KEY_F4",  293,
                    "KEY_F5",  294,
                    "KEY_F6",  295,
                    "KEY_F7",  296,
                    "KEY_F8",  297,
                    "KEY_F9",  298,
                    "KEY_F10",  299,
                    "KEY_F11",  300,
                    "KEY_F12",  301,
                    "KEY_F13",  302,
                    "KEY_F14",  303,
                    "KEY_F15",  304,
                    "KEY_F16",  305,
                    "KEY_F17",  306,
                    "KEY_F18",  307,
                    "KEY_F19",  308,
                    "KEY_F20",  309,
                    "KEY_F21",  310,
                    "KEY_F22",  311,
                    "KEY_F23",  312,
                    "KEY_F24",  313,
                    "KEY_F25",  314,
                    "KEY_KP_0",  320,
                    "KEY_KP_1",  321,
                    "KEY_KP_2",  322,
                    "KEY_KP_3",  323,
                    "KEY_KP_4",  324,
                    "KEY_KP_5",  325,
                    "KEY_KP_6",  326,
                    "KEY_KP_7",  327,
                    "KEY_KP_8",  328,
                    "KEY_KP_9",  329,
                    "KEY_KP_DECIMAL",  330,
                    "KEY_KP_DIVIDE",  331,
                    "KEY_KP_MULTIPLY",  332,
                    "KEY_KP_SUBTRACT",  333,
                    "KEY_KP_ADD",  334,
                    "KEY_KP_ENTER",  335,
                    "KEY_KP_EQUAL",  336,
                    "KEY_LEFT_SHIFT",  340,
                    "KEY_LEFT_CONTROL",  341,
                    "KEY_LEFT_ALT",  342,
                    "KEY_LEFT_SUPER",  343,
                    "KEY_RIGHT_SHIFT",  344,
                    "KEY_RIGHT_CONTROL",  345,
                    "KEY_RIGHT_ALT",  346,
                    "KEY_RIGHT_SUPER",  347,
                    "KEY_MENU",  348
                    );
        stateView[COMPONENTS_TBL]["Input"] = mProjectRuntime->getInputComponent();
    }

    void
    ScriptComponent::exposeAudioComponent
    ()
    {
        debugRegisteringClass("AudioComponent");
        sol::state_view stateView(State);
        stateView.new_usertype<AudioComponent>("AudioComponent");
        stateView[COMPONENTS_TBL]["Audio"] = mProjectRuntime->getAudioComponent();
    }

    void
    ScriptComponent::exposeScriptRuntime
    ()
    {
        // TODO
    }

    void
    ScriptComponent::exposeAudioRuntime
    ()
    {
        debugRegisteringClass("AudioRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<AudioRuntime>(
                    "AudioRuntime",
                    "getState",&AudioRuntime::getState,
                    "play",&AudioRuntime::play,
                    "pause",&AudioRuntime::pause,
                    "stop",&AudioRuntime::stop

                    );

        stateView.new_enum(
                    "AudioState",
                    "PLAYING", AudioStatus::AUDIO_STATUS_PLAYING,
                    "PAUSED",  AudioStatus::AUDIO_STATUS_PAUSED,
                    "STOPPED", AudioStatus::AUDIO_STATUS_STOPPED
                    );
    }

    void
    ScriptComponent::exposeSceneRuntime
    ()
    {
        debugRegisteringClass("SceneRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<SceneRuntime>(
                    "SceneRuntime",
                    "getCamera",&SceneRuntime::getCamera,
                    "getProjectRuntime", &SceneRuntime::getProjectRuntime,
                    "getEntityRuntimeByUuid",&SceneRuntime::getEntityRuntimeByUuid
                    );
    }

    void
    ScriptComponent::exposeGLM
    ()
    {
        debugRegisteringClass("GLM");
        sol::state_view stateView(State);

        auto vec3MultiplicationOverloads = sol::overload(
                    [](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1*v2; },
        [](const glm::vec3& v1, float f) -> glm::vec3 { return v1*f; },
        [](float f, const glm::vec3& v1) -> glm::vec3 { return f*v1; });

        auto vec3SubtractionOverloads = sol::overload(
                    [](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1-v2; },
        [](const glm::vec3& v1, float f) -> glm::vec3 { return v1-f; },
        [](float f, const glm::vec3& v1) -> glm::vec3 { return f-v1; });

        stateView.new_usertype<glm::vec3>(
                    "vec3",
                    sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
                    sol::meta_function::multiplication, vec3MultiplicationOverloads,
                    sol::meta_function::subtraction, vec3SubtractionOverloads,
                    "x", &glm::vec3::x,
                    "y", &glm::vec3::y,
                    "z", &glm::vec3::z
                    );

        stateView.new_usertype<glm::vec4>(
                    "vec4",
                    "x", &glm::vec4::x,
                    "y", &glm::vec4::y,
                    "z", &glm::vec4::z,
                    "w", &glm::vec4::w
                    );

        stateView.new_usertype<glm::quat>(
                    "quat",
                    sol::constructors<glm::quat(), glm::quat(float, float, float, float)>(),
                    "w", &glm::quat::w,
                    "x", &glm::quat::x,
                    "y", &glm::quat::y,
                    "z", &glm::quat::z
                    );

        auto mat4MultiplicationOverloads = sol::overload(
                    [](const glm::mat4& v1, const glm::mat4& v2) -> glm::mat4 { return v1*v2; },
        [](const glm::mat4& v1, float f) -> glm::mat4 { return v1*f; },
        [](float f, const glm::mat4& v1) -> glm::mat4 { return f*v1; });

        stateView.new_usertype<glm::mat4>(
                    "mat4",
                    sol::constructors<glm::mat4(), glm::mat4(float), glm::mat4(glm::mat4)>(),
                    sol::meta_function::multiplication, mat4MultiplicationOverloads
                    );

        stateView.set_function(
                    "translate",
                    [](const glm::mat4& m1, const glm::vec3& v1) -> glm::mat4{return glm::translate(m1,v1);});

        stateView.set_function(
                    "translate3f",
                    [](const glm::mat4& m1, float x, float y, float z) -> glm::mat4{return glm::translate(m1,vec3(x,y,z));});

        stateView.set_function(
                    "rotate",
                    [](const glm::mat4& m1, const float f1, const glm::vec3& v1) -> glm::mat4{return glm::rotate(m1,f1,v1);});

        stateView.set_function(
                    "scale",
                    [](const glm::mat4& m1, glm::vec3& v1) -> glm::mat4{return glm::scale(m1,v1);});

        stateView.set_function(
                    "mat4_cast",
                    [](const glm::quat& q1) -> glm::mat4{return glm::mat4_cast(q1);});

        stateView.set_function(
                    "matrix_col",
                    [](const glm::mat4& mtx, int col) -> glm::vec3{return  mtx[col];});
    }

    void
    ScriptComponent::exposeDefinition
    ()
    {
        debugRegisteringClass("Definitions");
        sol::state_view stateView(State);
        stateView.new_usertype<Definition>("Definition");
    }

    void
    ScriptComponent::exposeAnimationRuntime
    ()
    {
        debugRegisteringClass("AnimationRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<AnimationRuntime>(
                    "AnimationRuntime",
                    "run",&AnimationRuntime::run,
                    "pause",&AnimationRuntime::pause,
                    "reset",&AnimationRuntime::reset
                    );
    }

    void
    ScriptComponent::exposeOctronicMath()
    {
        sol::state_view stateView(State);

        debugRegisteringClass("Vector2");
        stateView.new_usertype<Vector2>(
                    "Vector2",
                    "x", &Vector2::x,
                    "y", &Vector2::y
                    );

        debugRegisteringClass("Vector3");
        stateView.new_usertype<Vector3>(
                    "Vector3",
                    "x",&Vector3::x,
                    "y",&Vector3::y,
                    "z",&Vector3::z
                    );

        debugRegisteringClass("Vector4");
        stateView.new_usertype<Vector4>(
                    "Vector4",
                    "w",&Vector4::w,
                    "x",&Vector4::x,
                    "y",&Vector4::y,
                    "z",&Vector4::z
                    );
    }

    void
    ScriptComponent::debugRegisteringClass
    (const string& className)
    {
        LOG_DEBUG( "Registering Class {}",  className );
    }

    void
    ScriptComponent::exposeAPI
    ()
    {
        exposeDefinition();
        exposeProjectRuntime();
        exposeProjectDirectory();
        exposeSceneRuntime();
        exposeEntityRuntime();
        exposeEvent();
        exposeTime();
        exposeTransform();
        exposeCamera();
        exposeGLM();
        exposeAudioComponent();
        exposeInputComponent();
        exposeGraphicsComponent();
        exposeWindowComponent();
        exposePhysicsComponent();
        exposeAnimationRuntime();
        exposeAudioRuntime();
        exposeModelRuntime();
        exposeLightRuntime();
        exposeShaderRuntime();
        exposePathRuntime();
        exposeScriptRuntime();
        exposePhysicsObjectRuntime();
        exposeOctronicMath();
    }

    vector<ScriptPrintListener*> ScriptComponent::PrintListeners;

    void
    ScriptComponent::AddPrintListener
    (ScriptPrintListener* listener)
    {
        PrintListeners.push_back(listener);
    }

    ScriptPrintListener::~ScriptPrintListener(){}

    lua_State* ScriptComponent::State = nullptr;

    const string ScriptComponent::COMPONENTS_TBL = "Components";
}
