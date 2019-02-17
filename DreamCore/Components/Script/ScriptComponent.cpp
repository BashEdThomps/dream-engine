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
#include "../../Project/Project.h"
#include "../Event.h"
#include "../Transform.h"
#include "../Time.h"
#include "../Path/PathRuntime.h"
#include "../Animation/AnimationRuntime.h"
#include "../Audio/AudioComponent.h"
#include "../Audio/AudioRuntime.h"
#include "../Graphics/Model/ModelRuntime.h"
#include "../Graphics/Camera.h"
#include "../Graphics/GraphicsComponent.h"
#include "../Graphics/NanoVGComponent.h"
#include "../Graphics/Light/LightRuntime.h"
#include "../Graphics/Shader/ShaderRuntime.h"
#include "../Input/InputComponent.h"
#include "../Physics/PhysicsComponent.h"
#include "../Physics/PhysicsObjectRuntime.h"
#include "../Window/WindowComponent.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../deps/angelscript/scriptstdstring/scriptstdstring.h"
#include "../../deps/angelscript/scriptbuilder/scriptbuilder.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Common/Math.h"

using std::stringstream;
using std::exception;
using std::cout;
using std::cerr;
using std::string;



static void whyYouFail(int r, int line)
{
    if (r<0)
    {
        string errStr;
        switch (r)
        {
            case asINVALID_DECLARATION:
                errStr = "Invalid Declaration";
                break;
            case asINVALID_NAME:
                errStr = "Invalid Name";
                break;
            case asNAME_TAKEN:
                errStr = "Name Taken";
                break;
            case asERROR:
                errStr = "Error: not a proper id";
                break;
            case asALREADY_REGISTERED:
                errStr = "Already Registered";
            break;
            default:
                errStr = "idk??";
                break;
        }
        cout << "On line: " << line << " errNo: " << r << " Reason: " << errStr << endl;
        assert(false);
    }
}

static void PrintCallback(const string& in)
{
    for (auto* pl : Dream::ScriptComponent::PrintListeners)
    {
        pl->onPrint(in);
    }
    cout << in << endl;
}

static void MessageCallback(const asSMessageInfo *msg, void *param)
{
  stringstream ss;
  if( msg->type == asMSGTYPE_WARNING )
  {
    ss << "WARN ";
  }
  else if( msg->type == asMSGTYPE_INFORMATION )
  {
    ss << "INFO ";
  }
  else
  {
    ss << "ERR ";
  }

  ss << "(" << msg->section << ") [" <<  msg->row << "," << msg->col << "] " << msg->message;
  cout << ss.str() << endl;

  for (auto* pl : Dream::ScriptComponent::PrintListeners)
    {
        pl->onPrint(ss.str());
    }
}

namespace Dream
{
    ScriptComponent::ScriptComponent
    (ProjectRuntime* runtime, ScriptCache* cache)
        : Component(runtime),
          mScriptCache(cache)
    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptingComponent");
        auto log = getLog();
        log->trace( "Constructing Object" );
        #endif
    }

    ScriptComponent::~ScriptComponent
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destroying Object");
        #endif
        lock();
        if (Engine != nullptr)
        {
            int r = asThreadCleanup(); assert(r>=0);
            r = Engine->ShutDownAndRelease(); assert(r>=0);
            asUnprepareMultithread();
            Engine = nullptr;
        }
        unlock();
    }

    bool
    ScriptComponent::init
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Initialising ScriptComponent" );
        #endif
        int r = asPrepareMultithread(); assert( r >= 0 );
        Engine = asCreateScriptEngine();
        r = Engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
        assert( r >= 0 );
        RegisterStdString(Engine);
        Engine->SetContextCallbacks(RequestContextCallback,ReturnContextToPool);
        #ifdef DREAM_LOG
        log->debug( "Got an AngelScript Engine");
        #endif
        exposeAPI();
        return true;
    }

    // API Exposure Methods ======================================================

    void
    ScriptComponent::exposeClasses
    ()
    {
        debugRegisteringClass("Dream Classes");
        int r;
        r = Engine->RegisterGlobalFunction("void print(string)", asFUNCTION(PrintCallback), asCALL_CDECL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("AnimationRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("AssetDefinition", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("AudioComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("AudioRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("Camera", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("CollisionData", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("Definition", 0, asOBJ_REF | asOBJ_NOCOUNT);
        r = Engine->RegisterObjectType("Event", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("GraphicsComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("InputComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("JoystickNavigation3D",0,asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("JoystickMapping",0,asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("JoystickState",0,asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("KeyboardState", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("MouseState", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("LightRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("ModelRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("NanoVGComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("PathRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("PhysicsComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("PhysicsObjectRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("Project", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("ProjectDirectory", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("ProjectRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("SceneObjectRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("SceneRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("ScriptRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("ShaderRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("Time", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("Transform", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("WindowComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);

        r = Engine->RegisterObjectType("Vector2", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("Vector3", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectType("Vector4", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r,__LINE__);

        // Enums
        r = Engine->RegisterEnum("KeyboardMapping");
    }

    void
    ScriptComponent::exposeAssetDefinition
    ()
    {
        debugRegisteringClass("AssetDefinition");
    }

    void
    ScriptComponent::exposeProjectRuntime
    ()
    {
        debugRegisteringClass("ProjectRuntime");
        int r;
        r = Engine->RegisterObjectMethod("ProjectRuntime", "int getUuid()",asMETHOD(ProjectRuntime,getUuid), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "string getName()",asMETHOD(ProjectRuntime,getName), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "string getNameAndUuidString()",asMETHOD(ProjectRuntime,getNameAndUuidString), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "Time@ getTime()",asMETHOD(ProjectRuntime,getTime), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "AssetDefinition@ getAssetDefinitionByUuid()",asMETHOD(ProjectRuntime,getAssetDefinitionByUuid), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "SceneObjectRuntime@ getSceneObjectByUuid()",asMETHOD(ProjectRuntime,getSceneObjectRuntimeByUuid), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "int getWindowWidth()",asMETHOD(ProjectRuntime,getWindowWidth), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "int getWindowHeight()",asMETHOD(ProjectRuntime,getWindowHeight), asCALL_THISCALL);whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "Project@ getProject()",asMETHOD(ProjectRuntime,getProject),asCALL_THISCALL); whyYouFail(r,__LINE__);

    }

    void
    ScriptComponent::exposeProject
    ()
    {
        debugRegisteringClass("Project");
        int r;
        r = Engine->RegisterObjectMethod("Project","ProjectDirectory@ getDirectory() const",asMETHOD(Project,getDirectory),asCALL_THISCALL); whyYouFail(r,__LINE__);
    }


    void
    ScriptComponent::exposeProjectDirectory
    ()
    {
        debugRegisteringClass("ProjectDirectory");
        int r;
        r = Engine->RegisterObjectMethod(
            "ProjectDirectory","string getAssetAbsolutePath(int) const",
            asMETHODPR(ProjectDirectory,getAssetAbsolutePath,(uint32_t) const,string),
            asCALL_THISCALL
        ); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeCamera
    ()
    {
        debugRegisteringClass("Camera");
        int r;
        r = Engine->RegisterObjectMethod("Camera", "void flyForward(float)",asMETHOD(Camera,flyForward), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Camera", "void flyBackward(float)",asMETHOD(Camera,flyBackward), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Camera", "void flyLeft(float)",asMETHOD(Camera,flyLeft), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Camera", "void flyRight(float)",asMETHOD(Camera,flyRight), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Camera", "void flyUp(float)",asMETHOD(Camera,flyUp), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Camera", "void flyDown(float)",asMETHOD(Camera,flyDown), asCALL_THISCALL); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposePathRuntime
    ()
    {
        debugRegisteringClass("PathRuntime");
    }

    void
    ScriptComponent::exposeGraphicsComponent
    ()
    {
        debugRegisteringClass("GraphicsComponent");
    }

    void
    ScriptComponent::exposeLightRuntime
    ()
    {
        debugRegisteringClass("LightRuntime");
    }

    void
    ScriptComponent::exposeShaderRuntime
    ()
    {
        debugRegisteringClass("ShaderRuntime");
    }

    void
    ScriptComponent::exposePhysicsComponent
    ()
    {
        debugRegisteringClass("PhysicsComponent");
    }

    void
    ScriptComponent::exposePhysicsObjectRuntime
    ()
    {
        debugRegisteringClass("PhysicsObjectRuntime");
        int r;
        //r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setCenterOfMassTransform(mat4)",asMETHODPR(PhysicsObjectRuntime,setCenterOfMassTransform,(mat4),void),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setCenterOfMassTransform(const Vector3@)",asMETHODPR(PhysicsObjectRuntime,setCenterOfMassTransform,(const Vector3&),void),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setCenterOfMassTransform(const Transform@)",asMETHODPR(PhysicsObjectRuntime,setCenterOfMassTransform,(const Transform&),void),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setCenterOfMassTransform(float,float,float)",asMETHODPR(PhysicsObjectRuntime,setCenterOfMassTransform,(float,float,float),void),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setLinearVelocity(float,float,float)",asMETHOD(PhysicsObjectRuntime,setLinearVelocity),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setAngularVelocity(float,float,float)",asMETHOD(PhysicsObjectRuntime,setAngularVelocity),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setLinearFactor(float,float,float)",asMETHOD(PhysicsObjectRuntime,setLinearFactor),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setAngularFactor(float,float,float)",asMETHOD(PhysicsObjectRuntime,setAngularFactor),asCALL_THISCALL); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeSceneObjectRuntime
    ()
    {
        debugRegisteringClass("SceneObjectRuntime");
        int r;
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "int getUuid()",asMETHOD(SceneObjectRuntime,getUuid), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "string getName()",asMETHOD(SceneObjectRuntime,getName), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "string getNameAndUuidString()",asMETHOD(SceneObjectRuntime,getNameAndUuidString), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "SceneRuntime@ getSceneRuntime()",asMETHOD(SceneObjectRuntime,getSceneRuntime), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "Transform@ getTransform()",asMETHOD(SceneObjectRuntime,getTransform), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "SceneObjectRuntime@ addChildFromTemplateUuid(int)",asMETHOD(SceneObjectRuntime,addChildFromTemplateUuid), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "bool hasPhysicsObjectRuntime()",asMETHOD(SceneObjectRuntime,hasPhysicsObjectRuntime), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "PhysicsObjectRuntime@ getPhysicsObjectRuntime()",asMETHOD(SceneObjectRuntime,getPhysicsObjectRuntime), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "bool hasAudioRuntime()",asMETHOD(SceneObjectRuntime,hasAudioRuntime), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "AudioRuntime@ getAudioRuntime()",asMETHOD(SceneObjectRuntime,getAudioRuntime), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "void setDeleted(bool)",asMETHOD(SceneObjectRuntime,setDeleted), asCALL_THISCALL); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeTransform
    ()
    {
        debugRegisteringClass("Transform");
        int r;
        r = Engine->RegisterObjectMethod("Transform", "void translate3f(float,float,float)",asMETHOD(Transform,translate3f), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Transform","Vector3@ getTranslation()",asMETHOD(Transform,getTranslation),asCALL_THISCALL); whyYouFail(r,__LINE__);
        //r = Engine->RegisterObjectMethod("Transform","mat4 getMatrix()",asMETHOD(Transform,getMatrix),asCALL_THISCALL); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeTime
    ()
    {
        debugRegisteringClass("Time");
        int r;
        r = Engine->RegisterObjectMethod("Time","double perSecond(double)",asMETHOD(Time,perSecond),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Time","int getCurrentFrameTime()",asMETHOD(Time,getCurrentFrameTime),asCALL_THISCALL); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeModelRuntime
    ()
    {
        debugRegisteringClass("ModelRuntime");
    }

    void
    ScriptComponent::exposeEvent
    ()
    {
        debugRegisteringClass("Event");
        int r;
        r = Engine->RegisterObjectMethod("Event","CollisionData@ getCollisionData()",asMETHOD(Event,getCollisionData),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("CollisionData","bool present",asOFFSET(CollisionData,present)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("CollisionData","float impulse",asOFFSET(CollisionData,impulse)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("CollisionData","Vector3@ position",asOFFSET(CollisionData,position)); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeWindowComponent
    ()
    {
        debugRegisteringClass("WindowComponent");
    }

    void
    ScriptComponent::exposeInputComponent
    ()
    {
        debugRegisteringClass("InputComponent");
        int r;
        // InputComponent
        r = Engine->RegisterObjectMethod("InputComponent","bool isKeyDown(int)",asMETHOD(InputComponent,isKeyDown),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("InputComponent","KeyboardState@ getKeyboardState()",asMETHOD(InputComponent,getKeyboardState),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("InputComponent","JoystickState@ getJoystickState()",asMETHOD(InputComponent,getJoystickState),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("InputComponent","MouseState@ getMouseState()",asMETHOD(InputComponent,getMouseState),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("InputComponent","JoystickNavigation3D@ getJoystickNavigation3D()",asMETHOD(InputComponent,getJoystickNavigation3D),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("InputComponent","JoystickMapping@ getJoystickMapping()",asMETHOD(InputComponent,getJoystickMapping),asCALL_THISCALL); whyYouFail(r,__LINE__);

        // Joystick State
        r = Engine->RegisterObjectMethod("JoystickState","float getAxisData(int)",asMETHOD(JoystickState,getAxisData),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("JoystickState","bool  getButtonData(int)",asMETHOD(JoystickState,getButtonData),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("JoystickState","bool clearsDeadzone(float)",asMETHOD(JoystickState,clearsDeadzone),asCALL_THISCALL); whyYouFail(r,__LINE__);

        // JoystickNavigation3D
        r = Engine->RegisterObjectMethod("JoystickNavigation3D","Vector2@ getHeading()",asMETHOD(JoystickNavigation3D,getHeading),asCALL_THISCALL); whyYouFail(r,__LINE__);

        // Joystick Mapping
        r = Engine->RegisterObjectProperty("JoystickMapping","int AnalogLeftXAxis",   asOFFSET(JoystickMapping,AnalogLeftXAxis)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int AnalogLeftYAxis",   asOFFSET(JoystickMapping,AnalogLeftYAxis)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int AnalogRightXAxis",  asOFFSET(JoystickMapping,AnalogRightXAxis)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int AnalogRightYAxis",  asOFFSET(JoystickMapping,AnalogRightYAxis)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int FaceButtonNorth",   asOFFSET(JoystickMapping,FaceButtonNorth)); whyYouFail(r,__LINE__); // Y
        r = Engine->RegisterObjectProperty("JoystickMapping","int FaceButtonEast",    asOFFSET(JoystickMapping,FaceButtonSouth)); whyYouFail(r,__LINE__); // B
        r = Engine->RegisterObjectProperty("JoystickMapping","int FaceButtonSouth",   asOFFSET(JoystickMapping,FaceButtonEast)); whyYouFail(r,__LINE__); // A
        r = Engine->RegisterObjectProperty("JoystickMapping","int FaceButtonWest",    asOFFSET(JoystickMapping,FaceButtonWest)); whyYouFail(r,__LINE__); // X
        r = Engine->RegisterObjectProperty("JoystickMapping","int FaceButtonSelect",  asOFFSET(JoystickMapping,FaceButtonSelect)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int FaceButtonStart",   asOFFSET(JoystickMapping,FaceButtonStart)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int FaceButtonHome",    asOFFSET(JoystickMapping,FaceButtonHome)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int ShoulderLeft",      asOFFSET(JoystickMapping,ShoulderLeft)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int ShoulderRight",     asOFFSET(JoystickMapping,ShoulderRight)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int TriggerLeftButton", asOFFSET(JoystickMapping,TriggerLeftButton)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int TriggerRightButton",asOFFSET(JoystickMapping,TriggerRightButton)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int AnalogLeftButton",  asOFFSET(JoystickMapping,AnalogLeftButton)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int AnalogRightButton", asOFFSET(JoystickMapping,AnalogRightButton)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int DPadNorth",         asOFFSET(JoystickMapping,DPadNorth)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int DPadWest",          asOFFSET(JoystickMapping,DPadWest)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int DPadSouth",         asOFFSET(JoystickMapping,DPadSouth)); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectProperty("JoystickMapping","int DPadEast",          asOFFSET(JoystickMapping,DPadEast)); whyYouFail(r,__LINE__);

        // Key Mappings
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_UNKNOWN",  -1); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_SPACE",  32);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_APOSTROPHE",  39);  // '
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_COMMA",  44); whyYouFail(r,__LINE__);  // ,
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_MINUS",  45); whyYouFail(r,__LINE__);  // -
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PERIOD",  46); whyYouFail(r,__LINE__);  // .
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_SLASH",  47); whyYouFail(r,__LINE__);  // /
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_0",  48); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_1",  49); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_2",  50); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_3",  51); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_4",  52); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_5",  53); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_6",  54); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_7",  55); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_8",  56); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_9",  57); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_SEMICOLON",  59); whyYouFail(r,__LINE__);  // ,
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_EQUAL",  61); whyYouFail(r,__LINE__);  // ",
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_A",  65); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_B",  66); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_C",  67); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_D",  68); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_E",  69); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F",  70); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_G",  71); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_H",  72); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_I",  73); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_J",  74); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_K",  75); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_L",  76); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_M",  77); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_N",  78); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_O",  79); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_P",  80); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_Q",  81); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_R",  82); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_S",  83); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_T",  84); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_U",  85); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_V",  86); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_W",  87); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_X",  88); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_Y",  89); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_Z",  90); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_BRACKET",  91); whyYouFail(r,__LINE__);  // [
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_BACKSLASH",  92); whyYouFail(r,__LINE__);  // Backslash
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_BRACKET",  93); whyYouFail(r,__LINE__);  // ]
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_GRAVE_ACCENT",  96); whyYouFail(r,__LINE__);  // `
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_WORLD_1",  161); whyYouFail(r,__LINE__); // non-US #1
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_WORLD_2",  162); whyYouFail(r,__LINE__); // non-US #2
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_ESCAPE",  256); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_ENTER",  257); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_TAB",  258); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_BACKSPACE",  259); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_INSERT",  260); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_DELETE",  261); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT",  262); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT",  263); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_DOWN",  264); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_UP",  265); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PAGE_UP",  266); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PAGE_DOWN",  267); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_HOME",  268); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_END",  269); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_CAPS_LOCK",  280); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_SCROLL_LOCK",  281); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_NUM_LOCK",  282); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PRINT_SCREEN",  283); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PAUSE",  284); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F1",  290); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F2",  291); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F3",  292); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F4",  293); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F5",  294); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F6",  295); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F7",  296); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F8",  297); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F9",  298); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F10",  299); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F11",  300); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F12",  301); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F13",  302); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F14",  303); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F15",  304); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F16",  305); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F17",  306); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F18",  307); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F19",  308); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F20",  309); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F21",  310); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F22",  311); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F23",  312); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F24",  313); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F25",  314); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_0",  320); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_1",  321); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_2",  322); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_3",  323); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_4",  324); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_5",  325); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_6",  326); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_7",  327); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_8",  328); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_9",  329); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_DECIMAL",  330); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_DIVIDE",  331); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_MULTIPLY",  332); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_SUBTRACT",  333); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_ADD",  334); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_ENTER",  335); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_EQUAL",  336); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_SHIFT",  340); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_CONTROL",  341); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_ALT",  342); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_SUPER",  343); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_SHIFT",  344); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_CONTROL",  345); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_ALT",  346); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_SUPER",  347); whyYouFail(r,__LINE__);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_MENU",  348); whyYouFail(r,__LINE__);
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
        int r;
        r = Engine->RegisterObjectMethod("AudioRuntime","void play()",asMETHOD(AudioRuntime,play),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("AudioRuntime","void pause()",asMETHOD(AudioRuntime,pause),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("AudioRuntime","void stop()",asMETHOD(AudioRuntime,stop),asCALL_THISCALL); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeAudioComponent
    ()
    {
        debugRegisteringClass("AudioRuntime");
    }


    void
    ScriptComponent::exposeNanoVG
    ()
    {
        debugRegisteringClass("NanoVG");
        int r;
        r = Engine->RegisterObjectMethod("NanoVGComponent","int CreateFont(string,string)",asMETHOD(NanoVGComponent,CreateFont),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("NanoVGComponent","void FontSize(float)",asMETHOD(NanoVGComponent,FontSize),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("NanoVGComponent","Vector4& TextBoxBounds(float, float, float, string)",asMETHODPR(NanoVGComponent,TextBoxBounds,(float, float, float, string),Vector4),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("NanoVGComponent","void TextBox(float, float, float, string)",asMETHODPR(NanoVGComponent,TextBox,(float,float,float, string),void),asCALL_THISCALL); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeSceneRuntime
    ()
    {
        debugRegisteringClass("SceneRuntime");
        int r;
        r = Engine->RegisterObjectMethod("SceneRuntime", "Camera@ getCamera()",asMETHOD(SceneRuntime,getCamera), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneRuntime", "SceneObjectRuntime@ getSceneObjectByUuid(int)",asMETHOD(SceneRuntime,getSceneObjectRuntimeByUuid), asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("SceneRuntime", "ProjectRuntime@ getProjectRuntime()",asMETHOD(SceneRuntime,getProjectRuntime), asCALL_THISCALL); whyYouFail(r,__LINE__);
    }

    void
    ScriptComponent::exposeGLM
    ()
    {
        debugRegisteringClass("Math Types");
        int r;
        // Vector2
        r = Engine->RegisterObjectMethod("Vector2","float x()",asMETHOD(Vector2,x),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector2","float y()",asMETHOD(Vector2,y),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector2","void setX(float)",asMETHOD(Vector2,setX),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector2","void setY(float)",asMETHOD(Vector2,setY),asCALL_THISCALL); whyYouFail(r,__LINE__);
        // Vector3
        r = Engine->RegisterObjectMethod("Vector3","float x()",asMETHOD(Vector3,x),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector3","float y()",asMETHOD(Vector3,y),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector3","float z()",asMETHOD(Vector3,z),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector3","void setX(float)",asMETHOD(Vector3,setX),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector3","void setY(float)",asMETHOD(Vector3,setY),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector3","void setZ(float)",asMETHOD(Vector3,setZ),asCALL_THISCALL); whyYouFail(r,__LINE__);
        // Vector4
        r = Engine->RegisterObjectMethod("Vector4","float w()",asMETHOD(Vector4,w),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector4","float x()",asMETHOD(Vector4,x),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector4","float y()",asMETHOD(Vector4,y),asCALL_THISCALL); whyYouFail(r,__LINE__);
        r = Engine->RegisterObjectMethod("Vector4","float z()",asMETHOD(Vector4,z),asCALL_THISCALL); whyYouFail(r,__LINE__);
    }


    void
    ScriptComponent::exposeDefinition
    ()
    {
        debugRegisteringClass("Definitions");
    }

    void
    ScriptComponent::exposeAnimationRuntime
    ()
    {
        debugRegisteringClass("AnimationRuntime");
        int r;
    }

    void
    ScriptComponent::debugRegisteringClass
    (const string& className)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Registering Class {}",  className );
        #endif
    }

    void
    ScriptComponent::exposeAPI
    ()
    {
        exposeClasses();
        exposeTime();
        exposeDefinition();
        exposeAssetDefinition();

        exposeEvent();
        exposeProject();
        exposeProjectRuntime();
        exposeProjectDirectory();

        exposeSceneRuntime();
        exposeSceneObjectRuntime();

        exposeTransform();
        exposeCamera();
        exposeGLM();

        exposeAudioComponent();
        exposeInputComponent();
        exposeGraphicsComponent();
        exposeWindowComponent();
        exposeNanoVG();
        exposePhysicsComponent();

        exposeAnimationRuntime();
        exposeAudioRuntime();
        exposeModelRuntime();
        exposeLightRuntime();
        exposeShaderRuntime();
        exposePathRuntime();
        exposeScriptRuntime();
        exposePhysicsObjectRuntime();
    }

    vector<ScriptPrintListener*> ScriptComponent::PrintListeners;

    void
    ScriptComponent::AddPrintListener
    (ScriptPrintListener* listener)
    {
        PrintListeners.push_back(listener);
    }

    ScriptPrintListener::~ScriptPrintListener(){}

    asIScriptEngine* ScriptComponent::Engine = nullptr;
    vector<asIScriptContext*> ScriptComponent::ContextPool;

    asIScriptContext*
    ScriptComponent::RequestContextCallback
    (asIScriptEngine *engine, void *param)
    {
      // Get a context from the pool, or create a new
      asIScriptContext *ctx = 0;
      if( ContextPool.size() )
      {
        ctx = *ContextPool.rbegin();
        ContextPool.pop_back();
      }
      else
      {
        ctx = Engine->CreateContext();
      }
      return ctx;
    }

    void
    ScriptComponent::ReturnContextToPool
    (asIScriptEngine *engine, asIScriptContext *ctx, void *param)
    {
      ContextPool.push_back(ctx);
      // Unprepare the context to free non-reusable resources
      ctx->Unprepare();
    }
}
