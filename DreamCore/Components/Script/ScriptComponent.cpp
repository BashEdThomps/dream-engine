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

using std::stringstream;
using std::exception;
using std::cout;
using std::cerr;
using std::string;

static mat4* mat4_translate3f(mat4 mtx, float x, float y, float z)
{
    static mat4 result = glm::translate(mtx,vec3(x,y,z));
    return &result;
}

static void mat4_delete(mat4* m)
{
    delete m;
}

static void vec3_delete(vec3* v)
{
    delete v;
}

static void vec4_delete(vec4* v)
{
    delete v;
}

static void whyYouFail(int r)
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
            default:
                errStr = "idk??";
                break;
        }
        cout << r << " " << errStr << endl;
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
            //int r = Engine->ShutDownAndRelease();
            //assert(r>=0);
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
        Engine = asCreateScriptEngine();
        int r = Engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
        assert( r >= 0 );
        RegisterStdString(Engine);
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
        r = Engine->RegisterGlobalFunction("void print(string)", asFUNCTION(PrintCallback), asCALL_CDECL); whyYouFail( r >= 0 );
        r = Engine->RegisterObjectType("AnimationRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("AssetDefinition", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("AudioComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("AudioRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("Camera", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("CollisionData", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("Definition", 0, asOBJ_REF | asOBJ_NOCOUNT);
        r = Engine->RegisterObjectType("Event", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("GraphicsComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("InputComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("KeyboardState", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("JoystickState", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("MouseState", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("LightRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("ModelRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("NanoVGComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("PathRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("PhysicsComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("PhysicsObjectRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("Project", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("ProjectDirectory", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("ProjectRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("SceneObjectRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("SceneRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("ScriptRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("ShaderRuntime", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("Time", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("Transform", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("WindowComponent", 0, asOBJ_REF | asOBJ_NOCOUNT); whyYouFail(r);
        r = Engine->RegisterObjectType("vec3", 0, asOBJ_REF | asOBJ_NOCOUNT ); whyYouFail(r);
        r = Engine->RegisterObjectType("vec4", 0, asOBJ_REF | asOBJ_NOCOUNT ); whyYouFail(r);
        r = Engine->RegisterObjectType("mat4", 0, asOBJ_REF | asOBJ_NOCOUNT ); whyYouFail(r);
        r = Engine->RegisterObjectType("quat", 0, asOBJ_REF | asOBJ_NOCOUNT ); whyYouFail(r);

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
        r = Engine->RegisterObjectMethod("ProjectRuntime", "Time@ getTime()",asMETHOD(ProjectRuntime,getTime), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "AssetDefinition@ getAssetDefinitionByUuid()",asMETHOD(ProjectRuntime,getAssetDefinitionByUuid), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "SceneObjectRuntime@ getSceneObjectByUuid()",asMETHOD(ProjectRuntime,getSceneObjectRuntimeByUuid), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "int getWindowWidth()",asMETHOD(ProjectRuntime,getWindowWidth), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "int getWindowHeight()",asMETHOD(ProjectRuntime,getWindowHeight), asCALL_THISCALL);whyYouFail(r);
        r = Engine->RegisterObjectMethod("ProjectRuntime", "Project@ getProject()",asMETHOD(ProjectRuntime,getProject),asCALL_THISCALL); whyYouFail(r);

    }

    void
    ScriptComponent::exposeProject
    ()
    {
        debugRegisteringClass("Project");
        int r;
        r = Engine->RegisterObjectMethod("Project","ProjectDirectory@ getDirectory() const",asMETHOD(Project,getDirectory),asCALL_THISCALL); whyYouFail(r);
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
        ); whyYouFail(r);
    }

    void
    ScriptComponent::exposeCamera
    ()
    {
        debugRegisteringClass("Camera");
        int r;
        r = Engine->RegisterObjectMethod("Camera", "void flyForward(float)",asMETHOD(Camera,flyForward), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("Camera", "void flyBackward(float)",asMETHOD(Camera,flyBackward), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("Camera", "void flyLeft(float)",asMETHOD(Camera,flyLeft), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("Camera", "void flyRight(float)",asMETHOD(Camera,flyRight), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("Camera", "void flyUp(float)",asMETHOD(Camera,flyUp), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("Camera", "void flyDown(float)",asMETHOD(Camera,flyDown), asCALL_THISCALL); whyYouFail(r);
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
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setCenterOfMassTransform(const mat4@)",asMETHODPR(PhysicsObjectRuntime,setCenterOfMassTransform,(const mat4&),void),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setCenterOfMassTransform(const Transform@)",asMETHODPR(PhysicsObjectRuntime,setCenterOfMassTransform,(const Transform&),void),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setLinearVelocity(float,float,float)",asMETHOD(PhysicsObjectRuntime,setLinearVelocity),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setAngularVelocity(float,float,float)",asMETHOD(PhysicsObjectRuntime,setAngularVelocity),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setLinearFactor(float,float,float)",asMETHOD(PhysicsObjectRuntime,setLinearFactor),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("PhysicsObjectRuntime","void setAngularFactor(float,float,float)",asMETHOD(PhysicsObjectRuntime,setAngularFactor),asCALL_THISCALL); whyYouFail(r);
    }

    void
    ScriptComponent::exposeSceneObjectRuntime
    ()
    {
        debugRegisteringClass("SceneObjectRuntime");
        int r;
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "SceneRuntime@ getSceneRuntime()",asMETHOD(SceneObjectRuntime,getSceneRuntime), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "Transform@ getTransform()",asMETHOD(SceneObjectRuntime,getTransform), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "SceneObjectRuntime@ addChildFromTemplateUuid(int)",asMETHOD(SceneObjectRuntime,addChildFromTemplateUuid), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "bool hasPhysicsObjectRuntime()",asMETHOD(SceneObjectRuntime,hasPhysicsObjectRuntime), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("SceneObjectRuntime", "PhysicsObjectRuntime@ getPhysicsObjectRuntime()",asMETHOD(SceneObjectRuntime,getPhysicsObjectRuntime), asCALL_THISCALL); whyYouFail(r);
    }

    void
    ScriptComponent::exposeTransform
    ()
    {
        debugRegisteringClass("Transform");
        int r;
        r = Engine->RegisterObjectMethod("Transform", "void translate3f(float,float,float)",asMETHOD(Transform,translate3f), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("Transform","vec3@ getTranslation()",asMETHOD(Transform,getTranslation),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("Transform","mat4@ getMatrix()",asMETHOD(Transform,getMatrix),asCALL_THISCALL); whyYouFail(r);
    }

    void
    ScriptComponent::exposeTime
    ()
    {
        debugRegisteringClass("Time");
        int r;
        r = Engine->RegisterObjectMethod("Time","double perSecond(double)",asMETHOD(Time,perSecond),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("Time","int getCurrentFrameTime()",asMETHOD(Time,getCurrentFrameTime),asCALL_THISCALL); whyYouFail(r);
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
        r = Engine->RegisterObjectMethod("InputComponent","void clearDeadzone()",asMETHOD(InputComponent,clearDeadzone),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("InputComponent","bool isKeyDown(int)",asMETHOD(InputComponent,isKeyDown),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("InputComponent","KeyboardState@ getKeyboardState()",asMETHOD(InputComponent,getKeyboardState),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("InputComponent","JoystickState@ getJoystickState()",asMETHOD(InputComponent,getJoystickState),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("InputComponent","MouseState@ getMouseState()",asMETHOD(InputComponent,getMouseState),asCALL_THISCALL); whyYouFail(r);

        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_UNKNOWN",  -1); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_SPACE",  32);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_APOSTROPHE",  39);  // '
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_COMMA",  44); whyYouFail(r);  // ,
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_MINUS",  45); whyYouFail(r);  // -
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PERIOD",  46); whyYouFail(r);  // .
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_SLASH",  47); whyYouFail(r);  // /
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_0",  48); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_1",  49); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_2",  50); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_3",  51); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_4",  52); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_5",  53); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_6",  54); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_7",  55); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_8",  56); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_9",  57); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_SEMICOLON",  59); whyYouFail(r);  // ,
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_EQUAL",  61); whyYouFail(r);  // ",
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_A",  65); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_B",  66); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_C",  67); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_D",  68); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_E",  69); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F",  70); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_G",  71); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_H",  72); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_I",  73); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_J",  74); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_K",  75); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_L",  76); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_M",  77); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_N",  78); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_O",  79); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_P",  80); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_Q",  81); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_R",  82); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_S",  83); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_T",  84); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_U",  85); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_V",  86); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_W",  87); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_X",  88); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_Y",  89); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_Z",  90); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_BRACKET",  91); whyYouFail(r);  // [
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_BACKSLASH",  92); whyYouFail(r);  // Backslash
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_BRACKET",  93); whyYouFail(r);  // ]
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_GRAVE_ACCENT",  96); whyYouFail(r);  // `
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_WORLD_1",  161); whyYouFail(r); // non-US #1
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_WORLD_2",  162); whyYouFail(r); // non-US #2
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_ESCAPE",  256); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_ENTER",  257); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_TAB",  258); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_BACKSPACE",  259); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_INSERT",  260); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_DELETE",  261); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT",  262); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT",  263); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_DOWN",  264); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_UP",  265); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PAGE_UP",  266); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PAGE_DOWN",  267); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_HOME",  268); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_END",  269); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_CAPS_LOCK",  280); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_SCROLL_LOCK",  281); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_NUM_LOCK",  282); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PRINT_SCREEN",  283); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_PAUSE",  284); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F1",  290); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F2",  291); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F3",  292); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F4",  293); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F5",  294); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F6",  295); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F7",  296); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F8",  297); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F9",  298); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F10",  299); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F11",  300); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F12",  301); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F13",  302); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F14",  303); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F15",  304); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F16",  305); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F17",  306); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F18",  307); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F19",  308); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F20",  309); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F21",  310); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F22",  311); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F23",  312); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F24",  313); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_F25",  314); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_0",  320); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_1",  321); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_2",  322); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_3",  323); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_4",  324); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_5",  325); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_6",  326); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_7",  327); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_8",  328); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_9",  329); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_DECIMAL",  330); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_DIVIDE",  331); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_MULTIPLY",  332); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_SUBTRACT",  333); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_ADD",  334); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_ENTER",  335); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_KP_EQUAL",  336); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_SHIFT",  340); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_CONTROL",  341); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_ALT",  342); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_LEFT_SUPER",  343); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_SHIFT",  344); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_CONTROL",  345); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_ALT",  346); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_RIGHT_SUPER",  347); whyYouFail(r);
        r = Engine->RegisterEnumValue("KeyboardMapping","KEY_MENU",  348); whyYouFail(r);
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
        r = Engine->RegisterObjectMethod("NanoVGComponent","int CreateFont(string,string)",asMETHOD(NanoVGComponent,CreateFont),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("NanoVGComponent","void FontSize(float)",asMETHOD(NanoVGComponent,FontSize),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("NanoVGComponent","vec4@ TextBoxBounds(float, float, float, string)",asMETHODPR(NanoVGComponent,TextBoxBounds,(float, float, float, string),vec4*),asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("NanoVGComponent","void TextBox(float, float, float, string)",asMETHODPR(NanoVGComponent,TextBox,(float,float,float, string),void),asCALL_THISCALL); whyYouFail(r);
    }

    void
    ScriptComponent::exposeSceneRuntime
    ()
    {
        debugRegisteringClass("SceneRuntime");
        int r;
        r = Engine->RegisterObjectMethod("SceneRuntime", "Camera@ getCamera()",asMETHOD(SceneRuntime,getCamera), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("SceneRuntime", "SceneObjectRuntime@ getSceneObjectByUuid(int)",asMETHOD(SceneRuntime,getSceneObjectRuntimeByUuid), asCALL_THISCALL); whyYouFail(r);
        r = Engine->RegisterObjectMethod("SceneRuntime", "ProjectRuntime@ getProjectRuntime()",asMETHOD(SceneRuntime,getProjectRuntime), asCALL_THISCALL); whyYouFail(r);
    }

    void
    ScriptComponent::exposeGLM
    ()
    {
        debugRegisteringClass("GLM");
        int r;
        r = Engine->RegisterGlobalFunction("mat4@ translate3f(mat4@, float,float,float)", asFUNCTION(mat4_translate3f), asCALL_CDECL); whyYouFail( r >= 0 );
        r = Engine->RegisterGlobalFunction("void mat4_delete(mat4@)",asFUNCTION(mat4_delete),asCALL_CDECL); whyYouFail(r);
        // vec3
        r = Engine->RegisterObjectProperty("vec3","float x",asOFFSET(vec3,x)); whyYouFail(r);
        r = Engine->RegisterObjectProperty("vec3","float y",asOFFSET(vec3,y)); whyYouFail(r);
        r = Engine->RegisterObjectProperty("vec3","float z",asOFFSET(vec3,z)); whyYouFail(r);
        r = Engine->RegisterGlobalFunction("void vec3_delete(vec3@)",asFUNCTION(vec4_delete),asCALL_CDECL); whyYouFail(r);
        // Vec4
        r = Engine->RegisterObjectProperty("vec4","float w",asOFFSET(vec4,w)); whyYouFail(r);
        r = Engine->RegisterObjectProperty("vec4","float x",asOFFSET(vec4,x)); whyYouFail(r);
        r = Engine->RegisterObjectProperty("vec4","float y",asOFFSET(vec4,y)); whyYouFail(r);
        r = Engine->RegisterObjectProperty("vec4","float z",asOFFSET(vec4,z)); whyYouFail(r);
        r = Engine->RegisterGlobalFunction("void vec4_delete(vec3@)",asFUNCTION(vec4_delete),asCALL_CDECL); whyYouFail(r);
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

    const string ScriptComponent::COMPONENTS_TBL = "Components";
}
