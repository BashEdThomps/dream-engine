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

#include "ScriptComponent.h"
#include "ScriptInstance.h"
#include "../Event.h"
#include "../Transform3D.h"
#include "../Time.h"
#include "../Path/PathComponent.h"
#include "../Path/PathInstance.h"
#include "../Audio/AudioComponent.h"
#include "../Audio/AudioInstance.h"
#include "../Graphics/Model/ModelInstance.h"
#include "../Graphics/Camera.h"
#include "../Graphics/GraphicsComponent.h"
#include "../Graphics/NanoVGComponent.h"
#include "../Graphics/Light/LightInstance.h"
#include "../Graphics/Shader/ShaderInstance.h"
#include "../Input/InputComponent.h"
#include "../Physics/PhysicsComponent.h"
#include "../Physics/PhysicsObjectInstance.h"
#include "../Window/IWindowComponent.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

#define SOL_CHECK_ARGUMENTS 1
#include "../../deps/sol2/sol.hpp"

using std::ostringstream;
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
    for (auto listener : Dream::ScriptComponent::PrintListeners)
    {
        listener->onPrint(out);
    }
    return 0;
}

static const struct luaL_Reg printlib [] =
{
  {"print", l_my_print},
  {nullptr, nullptr} /* end of array */
};

namespace Dream
{
    ScriptComponent::ScriptComponent
    (ProjectRuntime* runtime, ScriptCache* cache)
        : IComponent(),
          mScriptCache(cache),
          mProjectRuntime(runtime)
    {
        setLogClassName("LuaComponent");
        auto log = getLog();
        log->trace( "Constructing Object" );
    }

    ScriptComponent::~ScriptComponent
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );
        lua_close(State);
    }

    bool
    ScriptComponent::init
    ()
    {
        auto log = getLog();
        log->debug( "Initialising LuaComponent" );
        State = luaL_newstate();
        sol::state_view sView(State);
        sView.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::math,
            sol::lib::string
        );
        // Register print callback

        lua_getglobal(State, "_G");
        luaL_setfuncs(State, printlib, 0);
        lua_pop(State, 1);

        log->debug( "Got a sol state" );
        exposeAPI();
        return true;
    }

    void
    ScriptComponent::updateComponent
    (SceneRuntime*)
    {
        beginUpdate();
        auto log = getLog();
        log->debug( "Update Called" );

        for (auto inst : mScriptCache->getInstanceVector())
        {
            auto scriptObj = dynamic_cast<ScriptInstance*>(inst);
            scriptObj->executeOnInit();
            scriptObj->executeOnInput();
            scriptObj->executeOnEvent();
            scriptObj->executeOnUpdate();
        }
        endUpdate();
    }

    bool
    ScriptComponent::updateNanoVG
    ()
    {
        auto log = getLog();
        log->info( "UpdateNanoVG Called" );
        mProjectRuntime->getNanoVGComponent()->BeginFrame();
        for (auto inst : mScriptCache->getInstanceVector())
        {
            auto scriptObj = dynamic_cast<ScriptInstance*>(inst);
            scriptObj->executeOnNanoVG();
        }
        mProjectRuntime->getNanoVGComponent()->EndFrame();
        return true;
    }

    // API Exposure Methods ======================================================

    void
    ScriptComponent::exposeProjectRuntime
    ()
    {
        debugRegisteringClass("ProjectRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<ProjectRuntime>("ProjectRuntime",
            "getAudioComponent",&ProjectRuntime::getAudioComponent,
            "getGraphicsComponent",&ProjectRuntime::getGraphicsComponent,
            "getPhysicsComponent",&ProjectRuntime::getPhysicsComponent,
            "getWindowComponent",&ProjectRuntime::getWindowComponent,
            "getTime",&ProjectRuntime::getTime,
            "getAssetDefinition",&ProjectRuntime::getAssetDefinitionByUuid,
            "getAssetPath",&ProjectRuntime::getAssetAbsolutePath,
            "getSceneObject",&ProjectRuntime::getSceneObjectRuntimeByUuid,
            "windowWidth",&ProjectRuntime::getWindowWidth,
            "windowHeight",&ProjectRuntime::getWindowHeight
        );

        stateView["Runtime"] = mProjectRuntime;
    }

    void
    ScriptComponent::exposeCamera
    ()
    {
        debugRegisteringClass("Camera");
        sol::state_view stateView(State);
        stateView.new_usertype<Camera>("Camera",
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

        stateView.new_enum("CameraMovement",
            "FORWARD",  Constants::CAMERA_MOVEMENT_FORWARD,
            "BACKWARD", Constants::CAMERA_MOVEMENT_BACKWARD,
            "LEFT",     Constants::CAMERA_MOVEMENT_LEFT,
            "RIGHT",    Constants::CAMERA_MOVEMENT_RIGHT
        );
    }

    void
    ScriptComponent::exposePathComponent
    ()
    {
        debugRegisteringClass("PathComponent");
        sol::state_view stateView(State);
        stateView.new_usertype<PathComponent>("PathComponent");
    }

    void
    ScriptComponent::exposePathInstance
    ()
    {
        debugRegisteringClass("PathInstance");
        sol::state_view stateView(State);
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
    ScriptComponent::exposeGraphicsComponent
    ()
    {
        debugRegisteringClass("GraphicsComponent");
        sol::state_view stateView(State);
        stateView.new_usertype<GraphicsComponent>("GraphicsComponent");
    }

    void
    ScriptComponent::exposeLightInstance
    ()
    {
        debugRegisteringClass("LightInstance");
        sol::state_view stateView(State);
        stateView.new_usertype<LightInstance>("LightInstance");
    }

    void
    ScriptComponent::exposeShaderInstance
    ()
    {
        debugRegisteringClass("ShaderInstance");
        sol::state_view stateView(State);
        stateView.new_usertype<ShaderInstance>("ShaderInstance",
            "getUuid", &ShaderInstance::getUuid,
            "addUniform",&ShaderInstance::addUniform

        );

        debugRegisteringClass("ShaderUniform");

        stateView.new_usertype<ShaderUniform>("ShaderUniform");
        stateView.new_enum("UniformType",
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
    }

    void
    ScriptComponent::exposePhysicsObjectInstance
    ()
    {
        debugRegisteringClass("PhysicsObjectInstance");
        sol::state_view stateView(State);
        stateView.new_usertype<PhysicsObjectInstance>("PhysicsObjectInstance",
            "getUuid", &PhysicsObjectInstance::getUuid,

            "getMass", &PhysicsObjectInstance::getMass,
            "setMass", &PhysicsObjectInstance::setMass,

            "getLinearVelocity", &PhysicsObjectInstance::getLinearVelocity,
            "setLinearVelocity", &PhysicsObjectInstance::setLinearVelocity,

            "setLinearFactor", &PhysicsObjectInstance::setLinearFactor,
            "setAngularFactor", &PhysicsObjectInstance::setAngularFactor,

            "setAngularVelocity", &PhysicsObjectInstance::setAngularVelocity,

            "getRestitution", &PhysicsObjectInstance::getRestitution,
            "setRestitution", &PhysicsObjectInstance::setRestitution,

            "getFriction", &PhysicsObjectInstance::getFriction,
            "setFriction", &PhysicsObjectInstance::setFriction
        );
    }

    void
    ScriptComponent::exposeSceneObjectRuntime
    ()
    {
        debugRegisteringClass("SceneObjectRuntime");
        sol::state_view stateView(State);
        stateView.new_usertype<SceneObjectRuntime>("SceneObjectRuntime",
            "getName",&SceneObjectRuntime::getName,
            "getNameAndUuidString",&SceneObjectRuntime::getNameAndUuidString,
            "getChildByUuid",&SceneObjectRuntime::getChildRuntimeByUuid,
            "getParent",&SceneObjectRuntime::getParentRuntime,
            "setParent",&SceneObjectRuntime::setParentRuntime,

            "getCurrentTransform",&SceneObjectRuntime::getCurrentTransform,

            "getPath",&SceneObjectRuntime::getPathInstance,
            "getAudio",&SceneObjectRuntime::getAudioInstance,
            "getModel",&SceneObjectRuntime::getModelInstance,
            "getLight",&SceneObjectRuntime::getLightInstance,
            "getPhysicsObject",&SceneObjectRuntime::getPhysicsObjectInstance,

            "hasPath",&SceneObjectRuntime::hasPathInstance,
            "hasAudio",&SceneObjectRuntime::hasAudioInstance,
            "hasModel",&SceneObjectRuntime::hasModelInstance,
            "hasLight",&SceneObjectRuntime::hasLightInstance,
            "hasPhysicsObject",&SceneObjectRuntime::hasPhysicsObjectInstance,

            "getDeleted",&SceneObjectRuntime::getDeleted,
            "setDeleted",&SceneObjectRuntime::setDeleted,

            "getHidden",&SceneObjectRuntime::getHidden,
            "setHidden",&SceneObjectRuntime::setHidden,

            "addEvent",&SceneObjectRuntime::addEvent,

            "replaceAssetUuid",&SceneObjectRuntime::replaceAssetUuid

        );
    }

    void
    ScriptComponent::exposeTransform3D
    ()
    {
        debugRegisteringClass("Transform3D");
        sol::state_view stateView(State);
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
    ScriptComponent::exposeTime
    ()
    {
        debugRegisteringClass("Time");
        sol::state_view stateView(State);
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
    ScriptComponent::exposeAssimpModelInstance
    ()
    {
        debugRegisteringClass("AssimpModelInstance");
        sol::state_view stateView(State);
        stateView.new_usertype<ModelInstance>("AssimpModelInstance");
    }

    void
    ScriptComponent::exposeEvent
    ()
    {
        debugRegisteringClass("Event");
        sol::state_view stateView(State);
        stateView.new_usertype<Event>("Event",
            sol::constructors<Event(SceneObjectRuntime*,string)>(),
            "getSender",&Event::getSender,

            "getType",&Event::getType,

            "getString",&Event::getString,
            "setString",&Event::setString
        );
    }

    void
    ScriptComponent::exposeAudioComponent
    ()
    {
        debugRegisteringClass("AudioComponent");
        sol::state_view stateView(State);
        stateView.new_usertype<AudioComponent>("AudioComponent");
    }

    void
    ScriptComponent::exposeScriptInstance
    ()
    {
        // TODO
    }

    void
    ScriptComponent::exposeAudioInstance
    ()
    {
        debugRegisteringClass("AudioInstance");
        sol::state_view stateView(State);
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
    ScriptComponent::exposeNanoVG
    ()
    {
        debugRegisteringClass("NanoVG");
        sol::state_view stateView(State);

        stateView.new_usertype<NVGcolor>("NVGcolor");
        stateView.new_usertype<NVGpaint>("NVGpaint");
        stateView.new_usertype<NVGglyphPosition>("NVGglyphPosition");
        stateView.new_usertype<NVGtextRow>("NVGtextRow");

        stateView.new_enum("NVGsolidity",
            "NVG_SOLID",NVG_SOLID,
            "NVG_HOLE",NVG_HOLE
        );
        stateView.new_enum("NVGImageFlags",
            "NVG_IMAGE_GENERATE_MIPMAPS",NVG_IMAGE_GENERATE_MIPMAPS,
            "NVG_IMAGE_REPEATX",NVG_IMAGE_REPEATX,
            "NVG_IMAGE_REPEATY",NVG_IMAGE_REPEATY,
            "NVG_IMAGE_FLIPY",NVG_IMAGE_FLIPY,
            "NVG_IMAGE_PREMULTIPLIED",NVG_IMAGE_PREMULTIPLIED,
            "NVG_IMAGE_NEAREST",NVG_IMAGE_NEAREST
        );
        stateView.new_usertype<NanoVGComponent>("NanoVGComponent",
            "GlobalCompositeOperation",&NanoVGComponent::GlobalCompositeOperation,
            "GlobalCompositeBlendFunc",&NanoVGComponent::GlobalCompositeBlendFunc,
            "GlobalCompositeBlendFuncSeparate",&NanoVGComponent::GlobalCompositeBlendFuncSeparate,
            "RGB",&NanoVGComponent::_RGB,
            "RGBf",&NanoVGComponent::RGBf,
            "RGBA",&NanoVGComponent::RGBA,
            "RGBAf",&NanoVGComponent::RGBAf,
            "LerpRGBA",&NanoVGComponent::LerpRGBA,
            "TransRGBA",&NanoVGComponent::TransRGBA,
            "TransRGBAf",&NanoVGComponent::TransRGBAf,
            "HSL",&NanoVGComponent::HSL,
            "HSLA",&NanoVGComponent::HSLA,
            "Save",&NanoVGComponent::Save,
            "Restore",&NanoVGComponent::Restore,
            "Reset",&NanoVGComponent::Reset,
            "ShapeAntiAlias",&NanoVGComponent::ShapeAntiAlias,
            "StrokeColor",&NanoVGComponent::StrokeColor,
            "StrokePaint",&NanoVGComponent::StrokePaint,
            "FillColor",&NanoVGComponent::FillColor,
            "FillPaint",&NanoVGComponent::FillPaint,
            "MiterLimit",&NanoVGComponent::MiterLimit,
            "StrokeWidth",&NanoVGComponent::StrokeWidth,
            "LineCap",&NanoVGComponent::LineCap,
            "LineJoin",&NanoVGComponent::LineJoin,
            "GlobalAlpha",&NanoVGComponent::GlobalAlpha,
            "ResetTransform",&NanoVGComponent::ResetTransform,
            "Transform",&NanoVGComponent::Transform,
            "Translate",&NanoVGComponent::Translate,
            "Rotate",&NanoVGComponent::Rotate,
            "SkewX",&NanoVGComponent::SkewX,
            "SkewY",&NanoVGComponent::SkewY,
            "Scale;",&NanoVGComponent::Scale,
            "CurrentTransform",&NanoVGComponent::CurrentTransform,
            "TransformIdentity",&NanoVGComponent::TransformIdentity,
            "TransformTranslate",&NanoVGComponent::TransformTranslate,
            "TransformScale",&NanoVGComponent::TransformScale,
            "TransformRotate",&NanoVGComponent::TransformRotate,
            "TransformSkewX",&NanoVGComponent::TransformSkewX,
            "TransformSkewY",&NanoVGComponent::TransformSkewY,
            "TransformMultiply",&NanoVGComponent::TransformMultiply,
            "TransformPremultiply",&NanoVGComponent::TransformPremultiply,
            "TransformInverse",&NanoVGComponent::TransformInverse,
            "TransformPoint",&NanoVGComponent::TransformPoint,
            "DegToRad",&NanoVGComponent::DegToRad,
            "RadToDeg",&NanoVGComponent::RadToDeg,
            "CreateImage",&NanoVGComponent::CreateImage,
            "CreateImageMem",&NanoVGComponent::CreateImageMem,
            "CreateImageRGBA",&NanoVGComponent::CreateImageRGBA,
            "UpdateImage",&NanoVGComponent::UpdateImage,
            "ImageSize",&NanoVGComponent::ImageSize,
            "DeleteImage",&NanoVGComponent::DeleteImage,
            "LinearGradient",&NanoVGComponent::LinearGradient,
            "BoxGradient",&NanoVGComponent::BoxGradient,
            "RadialGradient",&NanoVGComponent::RadialGradient,
            "ImagePattern",&NanoVGComponent::ImagePattern,
            "Scissor",&NanoVGComponent::Scissor,
            "IntersectScissor",&NanoVGComponent::IntersectScissor,
            "ResetScissor",&NanoVGComponent::ResetScissor,
            "BeginPath",&NanoVGComponent::BeginPath,
            "MoveTo",&NanoVGComponent::MoveTo,
            "LineTo",&NanoVGComponent::LineTo,
            "BezierTo",&NanoVGComponent::BezierTo,
            "QuadTo",&NanoVGComponent::QuadTo,
            "ArcTo",&NanoVGComponent::ArcTo,
            "ClosePath",&NanoVGComponent::ClosePath,
            "PathWinding",&NanoVGComponent::PathWinding,
            "Arc",&NanoVGComponent::Arc,
            "Rect",&NanoVGComponent::Rect,
            "RoundedRect",&NanoVGComponent::RoundedRect,
            "RoundedRectVarying",&NanoVGComponent::RoundedRectVarying,
            "Ellipse",&NanoVGComponent::Ellipse,
            "Circle",&NanoVGComponent::Circle,
            "Fill",&NanoVGComponent::Fill,
            "Stroke",&NanoVGComponent::Stroke,
            "CreateFont",&NanoVGComponent::CreateFont,
            "CreateFontMem",&NanoVGComponent::CreateFontMem,
            "FindFont",&NanoVGComponent::FindFont,
            "AddFallbackFontId",&NanoVGComponent::AddFallbackFontId,
            "AddFallbackFont",&NanoVGComponent::AddFallbackFont,
            "FontSize",&NanoVGComponent::FontSize,
            "FontBlur",&NanoVGComponent::FontBlur,
            "TextLetterSpacing",&NanoVGComponent::TextLetterSpacing,
            "TextLineHeight",&NanoVGComponent::TextLineHeight,
            "TextAlign",&NanoVGComponent::TextAlign,
            "FontFaceId",&NanoVGComponent::FontFaceId,
            "FontFace",&NanoVGComponent::FontFace,
            "Text",static_cast<float (NanoVGComponent::*)(float,float,const char*)>(&NanoVGComponent::Text),
            "TextBox",&NanoVGComponent::TextBox,
            "TextBounds",&NanoVGComponent::TextBounds,
            "TextBoxBounds",&NanoVGComponent::TextBoxBounds,
            "TextGlyphPositions",&NanoVGComponent::TextGlyphPositions,
            "TextMetrics",&NanoVGComponent::TextMetrics,
            "TextBreakLines",&NanoVGComponent::TextBreakLines
        );

        stateView["NanoVG"] = mProjectRuntime->getNanoVGComponent();
    }

    void
    ScriptComponent::exposeGLM
    ()
    {
        debugRegisteringClass("GLM");
        sol::state_view stateView(State);

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
    ScriptComponent::exposeIDefinition
    ()
    {
        sol::state_view stateView(State);
        stateView.new_usertype<IDefinition>("IDefinition");

    }

    void
    ScriptComponent::debugRegisteringClass
    (string className)
    {
        auto log = getLog();
        log->debug( "Registering Class {}",  className );
    }

    void
    ScriptComponent::exposeAPI
    ()
    {
        // Definitions
        exposeIDefinition();
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
        exposeLightInstance();
        exposeShaderInstance();
        exposeGLM();
        exposeNanoVG();
        // Path
        exposePathComponent();
        exposePathInstance();
        // Script
        exposeScriptInstance();
        // Physics
        exposePhysicsComponent();
        exposePhysicsObjectInstance();
    }

    vector<LuaPrintListener*> ScriptComponent::PrintListeners;

    void
    ScriptComponent::AddPrintListener
    (LuaPrintListener* listener)
    {
        PrintListeners.push_back(listener);
    }

    LuaPrintListener::~LuaPrintListener(){}

    lua_State* ScriptComponent::State = nullptr;
} // End of Dream

